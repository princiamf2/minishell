/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:51:31 by michel            #+#    #+#             */
/*   Updated: 2025/03/18 15:21:44 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int *create_pipes(int n)
{
	int *pipes;
	int i;

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

int create_child_process(t_command *cmd, int i, t_pipe_info *pi, t_data *data)
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	if (pid == 0)
	{
		fprintf(stderr, "[debugger] Enfant %d lancé pour commande: %s\n", i, cmd->args[0]);
		execute_child(cmd, i, pi, data);
	}
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

int execute_pipeline(t_command *pipeline, t_data *data)
{
	int n;
	t_pipe_info pi;
	int status;

	n = count_commands(pipeline);
	pi.n = n;
	pi.pipes = create_pipes(n);
	if (n > 1 && !pi.pipes)
		return (-1);
	status = fork_pipeline(pipeline, &pi, data);
	if (pi.pipes)
		free(pi.pipes);
	return status;
}

void print_env_array(char **env_array)
{
	int i = 0;
	if (!env_array)
	{
		printf("env_array est NULL\n");
		return;
	}
	while (env_array[i])
	{
		printf("%s\n", env_array[i]);
		i++;
	}
}
