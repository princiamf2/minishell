/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:51:31 by michel            #+#    #+#             */
/*   Updated: 2025/03/27 15:57:46 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	execute_builtin_or_command(t_command *cmd, t_data *data)
{
	char	*exec_path;

	if (is_builtins(cmd->args[0]))
		exit(execute_builtin(cmd->args, data));
	if (ft_strchr(cmd->args[0], '/') != NULL)
		exec_path = ft_strdup(cmd->args[0]);
	else
		exec_path = find_excutable(cmd->args[0], data->env);
	if (!exec_path)
		exit(127);
	execve(exec_path, cmd->args, env_to_array(data->env));
	perror("execve");
	exit(126);
}

int	execute_child(t_command *cmd, int index, t_pipe_info *pi, t_data *data)
{
	setup_pipes_for_child(index, pi);
	close_unused_pipes(pi);
	if (handle_redirection(cmd) < 0)
		exit(1);
	execute_builtin_or_command(cmd, data);
	return (0);
}
