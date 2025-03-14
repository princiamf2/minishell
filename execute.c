/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:48:36 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/14 19:39:11 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_excutable(const char *cmd)
{
	ft_putstr_fd("rentre dans find_excutable\n", 1);
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

int execute_command(t_command *cmd, t_data *data)
{
	ft_putstr_fd("rentre dans execute_command\n", 1);
	char *exec_path;
	int status;
	pid_t pid;

	if (is_builtins(cmd->args[0]))
		return (execute_builtin_with_redir(cmd, data));
	if (handle_redirection(cmd) < 0)
		return (1);
	if (ft_strchr(cmd->args[0], '/') != NULL)
		exec_path = ft_strdup(cmd->args[0]);
	else
		exec_path = find_excutable(cmd->args[0]);
	if (!exec_path)
	{
		ft_putstr_fd("exec_path pas trouver\n", 2);
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
		ft_putstr_fd("pid = 0\n", 1);
		if (execve(exec_path, cmd->args, env_to_array(data->env)) == -1)
		{
			ft_putstr_fd("rentre bien dans execve\n", 1);
			perror("execve");
			exit(1);
		}
	}
	waitpid(pid, &status, 0);
	free(exec_path);
	ft_putstr_fd("sort de execute_command\n", 1);
	return (WIFEXITED(status)) ? WEXITSTATUS(status) : 1;
}

int execute_full_command(t_command *cmd, t_data *data)
{
	ft_putstr_fd("rentre dans full_command\n", 1);
    if (cmd->next_pipe)
        return execute_pipeline(cmd, data);
    else
        return execute_command(cmd, data);
}
