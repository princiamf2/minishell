/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_fork.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:53:30 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:55:57 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	fork_and_execute(char *exec_path, t_command *cmd, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		child_process_exec(exec_path, cmd, data);
	}
	return (parent_process_wait(pid));
}

void	child_process_exec_no_redir(char *exec_path, t_command *cmd,
		t_data *data)
{
	char	**env_array;

	env_array = env_to_array(data->env);
	if (!env_array)
		exit(1);
	execve(exec_path, cmd->args, env_array);
	perror("minishell: execve");
	free_str_array_child(env_array);
	exit(126);
}

int	fork_and_execute_no_redir(char *exec_path, t_command *cmd, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("minishell: fork");
		return (1);
	}
	if (pid == 0)
	{
		child_process_exec_no_redir(exec_path, cmd, data);
	}
	return (parent_process_wait(pid));
}

void	child_process_exec(char *exec_path, t_command *cmd, t_data *data)
{
	char	**env_array;

	if (handle_redirection(cmd) < 0)
		exit(1);
	env_array = env_to_array(data->env);
	if (!env_array)
		exit(1);
	execve(exec_path, cmd->args, env_array);
	perror("minishell: execve");
	free_str_array_child(env_array);
	exit(126);
}

int	parent_process_wait(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
