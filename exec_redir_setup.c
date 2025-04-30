/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redir_setup.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:05:30 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:57:51 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_no_command_arguments(t_command *cmd, int saved_stdin,
		int saved_stdout)
{
	if (!cmd->args || !cmd->args[0])
	{
		if (handle_redirection(cmd) < 0)
		{
			close(saved_stdout);
			return (1);
		}
		restore_stdin(saved_stdin);
		if (dup2(saved_stdout, STDOUT_FILENO) < 0)
			perror("dup2 stdout");
		close(saved_stdout);
		return (0);
	}
	return (-1);
}

bool	save_original_fds(int *saved_stdin_ptr, int *saved_stdout_ptr)
{
	*saved_stdin_ptr = dup(STDIN_FILENO);
	*saved_stdout_ptr = dup(STDOUT_FILENO);
	if (*saved_stdin_ptr < 0 || *saved_stdout_ptr < 0)
	{
		perror("dup");
		if (*saved_stdin_ptr >= 0)
			close(*saved_stdin_ptr);
		if (*saved_stdout_ptr >= 0)
			close(*saved_stdout_ptr);
		*saved_stdin_ptr = -1;
		*saved_stdout_ptr = -1;
		return (false);
	}
	return (true);
}

void	restore_original_fds(int saved_stdin, int saved_stdout)
{
	if (saved_stdin >= 0)
	{
		if (dup2(saved_stdin, STDIN_FILENO) < 0)
			perror("dup2 stdin restore");
		close(saved_stdin);
	}
	if (saved_stdout >= 0)
	{
		if (dup2(saved_stdout, STDOUT_FILENO) < 0)
			perror("dup2 stdout restore");
		close(saved_stdout);
	}
}

void	restore_stdin(int saved)
{
	if (dup2(saved, STDIN_FILENO) < 0)
		perror("dup2");
	close(saved);
}
