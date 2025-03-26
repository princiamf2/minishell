/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:41:07 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/26 19:33:59 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
