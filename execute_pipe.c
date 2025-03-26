/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:51:31 by michel            #+#    #+#             */
/*   Updated: 2025/03/26 19:32:37 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_builtin_or_command(t_command *cmd, t_data *data)
{
	char	*exec_path;

	if (is_builtins(cmd->args[0]))
		exit(execute_builtin(cmd->args, data));
	if (ft_strchr(cmd->args[0], '/') != NULL)
		exec_path = ft_strdup(cmd->args[0]);
	else
		exec_path = find_excutable(cmd->args[0]);
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
