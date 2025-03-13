/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:48:36 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/13 00:54:48 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "minishell.h"
#include <sys/wait.h>
#include <unistd.h>

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

int execute_command(t_command *cmd, char **envp)
{
	char *exec_path;
	int status;
	pid_t pid;

	if (handle_redirection(cmd) < 0)
		return (1);
	if (is_builtins(cmd->args[0]))
		return (execute_builtin(cmd->args, envp));
	if (ft_strchr(cmd->args[0], '/') != NULL)
		exec_path = ft_strdup(cmd->args[0]);
	else
		exec_path = find_excutable(cmd->args[0]);
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
		if (execve(exec_path, cmd->args, envp) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	waitpid(pid, &status, 0);
	free(exec_path);
	return (WIFEXITED(status)) ? WEXITSTATUS(status) : 1;
}

int execute_full_command(t_command *cmd, t_env *env, char **envp)
{
    if (cmd->next_pipe)
        return execute_pipeline(cmd, env);
    else
        return execute_command(cmd, envp);
}
