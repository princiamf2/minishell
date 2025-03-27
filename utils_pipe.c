/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:09:04 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/25 14:37:15 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	create_child_process(t_command *cmd, int i, t_pipe_info *pi, t_data *data)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
		execute_child(cmd, i, pi, data);
	return (pid);
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

int	fork_pipeline(t_command *pipeline, t_pipe_info *pi, t_data *data)
{
	int			i;
	t_command	*cmd;
	pid_t		pid;

	cmd = pipeline;
	i = 0;
	while (cmd)
	{
		pid = create_child_process(cmd, i, pi, data);
		if (pid < 0)
			return (-1);
		cmd = cmd->next_pipe;
		i++;
	}
	close_pipeline_pipes(pi);
	return (wait_for_children(pi->n));
}

int	execute_pipeline(t_command *pipeline, t_data *data)
{
	int			n;
	t_pipe_info	pi;
	int			status;

	n = count_commands(pipeline);
	pi.n = n;
	pi.pipes = create_pipes(n);
	if (n > 1 && !pi.pipes)
		return (-1);
	status = fork_pipeline(pipeline, &pi, data);
	if (pi.pipes)
		free(pi.pipes);
	return (status);
}
