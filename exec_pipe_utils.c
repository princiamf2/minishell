/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:51:31 by michel            #+#    #+#             */
/*   Updated: 2025/04/30 13:41:16 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_pipes_for_child(int index, t_pipe_info *pi)
{
	if (index > 0)
	{
		if (dup2(pi->pipes[(index - 1) * 2], STDIN_FILENO) < 0)
			exit(1);
	}
	if (index < pi->n - 1)
	{
		if (dup2(pi->pipes[index * 2 + 1], STDOUT_FILENO) < 0)
			exit(1);
	}
}

void	close_unused_pipes(t_pipe_info *pi)
{
	int	j;

	j = 0;
	while (j < 2 * (pi->n - 1))
	{
		close(pi->pipes[j]);
		j++;
	}
}

void	close_pipeline_pipes(t_pipe_info *pi)
{
	int	i;

	if (pi->pipes)
	{
		i = 0;
		while (i < 2 * (pi->n - 1))
		{
			close(pi->pipes[i]);
			i++;
		}
	}
}

int	wait_for_children(int n)
{
	int	i;
	int	status;
	int	last_status;

	last_status = 0;
	status = 0;
	i = 0;
	while (i < n)
	{
		wait(&status);
		last_status = status;
		i++;
	}
	return (WEXITSTATUS(last_status));
}

void	execute_builtin_or_command(t_command *cmd, t_data *data)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		exit(1);
	if (is_builtins(cmd->args[0]))
		exit(execute_builtin(cmd->args, data));
	execute_external_in_child(cmd, data);
}
