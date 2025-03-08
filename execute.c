/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:48:36 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/08 20:28:05 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_excutable(const char *cmd)
{
	char	*path_env;
	char	*paths;
	char	*token;
	char	*temp;
	char	*full_path;

	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_strdup(path_env);
	if (!paths)
		return (NULL);
	token = ft_strtok(paths, ":");
	while (token)
	{
		temp = ft_strjoin(token, "/");
		if (!temp)
		{
			free(paths);
			return (NULL);
		}
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
		{
			free(paths);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			free(paths);
			return (full_path);
		}
		free(full_path);
		token = ft_strtok(NULL, ":");
	}
	free(paths);
	return (NULL);
}

int execute_command(char **args, char **envp)
{
	char *exec_path;
	int status;
	pid_t pid;

	if (!args || !args[0])
		return (1);
	if (is_builtins(args[0]))
		return (execute_builtin(args, envp));
	else
	{
		if (ft_strchr(args[0], '/') != NULL)
			exec_path = ft_strdup(args[0]);
		else
			exec_path = find_excutable(args[0]);
		if (!exec_path)
		{
			ft_putstr_fd("commande pas trouver\n", 2);
			return (127);
		}
		pid = fork();
		if (pid < 0)
		{
			perror("fork");
			free(exec_path);
			return (1);
		}
		if (pid == 0)
		{
			if (execve(exec_path, args, envp) == -1)
			{
				perror("execve");
				exit(1);
			}
		}
		waitpid(pid, &status, 0);
		free(exec_path);
		return (WIFEXITED(status)) ? WEXITSTATUS(status) : 1;
	}
}
