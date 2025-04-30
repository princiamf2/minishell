/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_subshell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:02:39 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:59:51 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_subshell(t_command *cmd, t_data *data)
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
		child_process_subshell(cmd, data);
	}
	else
	{
		return (parent_process_subshell(pid));
	}
	return (1);
}

void	child_process_subshell(t_command *cmd, t_data *data)
{
	int	exit_status;

	if (handle_redirection(cmd) < 0)
		exit(1);
	exit_status = execute_cmdlist(cmd->subshell_ast, data);
	exit(exit_status);
}

int	parent_process_subshell(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		return (WEXITSTATUS(status));
	}
	else if (WIFSIGNALED(status))
	{
		return (128 + WTERMSIG(status));
	}
	else
	{
		return (1);
	}
}
