/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:09:04 by mm-furi           #+#    #+#             */
/*   Updated: 2025/05/15 13:48:49 by mm-furi          ###   ########.fr       */
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

int	*create_pipes(int n)
{
	int	*pipes;
	int	i;

	if (n < 2)
		return (NULL);
	pipes = malloc(2 * (n - 1) * sizeof(int));
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < n - 1)
	{
		if (pipe(&pipes[i * 2]) < 0)
		{
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

int	execute_child(t_command *cmd, int index, t_pipe_info *pi, t_data *data)
{
	if (cmd->heredoc || cmd->input || cmd->output)
	{
		if (handle_redirection(cmd) < 0)
			exit(1);
	}
	setup_pipes_for_child(index, pi);
	close_unused_pipes(pi);
	execute_builtin_or_command(cmd, data);
	return (0);
}
