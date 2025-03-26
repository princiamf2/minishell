/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:48:36 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/26 19:39:42 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_excutable(const char *cmd)
{
	char	*paths;
	char	*full_path;

	paths = get_path_env();
	if (!paths)
		return (NULL);
	full_path = search_executable_in_paths(paths, cmd);
	free(paths);
	return (full_path);
}

int	fork_and_execute(char *exec_path, t_command *cmd, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(exec_path);
		return (1);
	}
	if (pid == 0)
	{
		if (handle_redirection(cmd) < 0)
			exit(1);
		if (execve(exec_path, cmd->args, env_to_array(data->env)) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_external_command(t_command *cmd, t_data *data)
{
	char	*exec_path;
	int		status;

	exec_path = get_executable_path(cmd);
	if (!exec_path)
		return (127);
	status = fork_and_execute(exec_path, cmd, data);
	free(exec_path);
	return (status);
}

int	execute_command(t_command *cmd, t_data *data)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	saved_stdin = save_stdin();
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0)
	{
		perror("dup");
		return (1);
	}
	if (is_builtins(cmd->args[0]))
		return (execute_builtin_with_redir(cmd, data));
	status = execute_external_command(cmd, data);
	restore_stdin(saved_stdin);
	if (dup2(saved_stdout, STDOUT_FILENO) < 0)
		perror("dup2 stdout");
	close(saved_stdout);
	return (status);
}

int	execute_full_command(t_command *cmd, t_data *data)
{
	if (cmd->next_pipe)
		return (execute_pipeline(cmd, data));
	else
		return (execute_command(cmd, data));
}
