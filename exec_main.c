/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:14:29 by michel            #+#    #+#             */
/*   Updated: 2025/05/14 18:40:42 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cmdlist(t_cmdlist *commands, t_data *data)
{
	int			last_status;
	t_cmdlist	*node;

	last_status = data->exit_status;
	node = commands;
	if (prepare_all_heredocs(commands) < 0)
		return (1);
	while (node)
	{
		if (!node->andor)
		{
			last_status = 1;
			node = node->next;
			continue ;
		}
		last_status = execute_andor(node->andor, data);
		node = node->next;
	}
	return (last_status);
}

int	execute_andor(t_andor *list, t_data *data)
{
	int	result;

	if (!list || !list->pipeline)
		return (1);
	result = execute_full_command(list->pipeline, data);
	list = list->next;
	while (list)
	{
		if (list->op == AND_IF && result != 0)
			break ;
		if (list->op == OR_IF && result == 0)
			break ;
		if (!list->pipeline)
		{
			result = 1;
			break ;
		}
		result = execute_full_command(list->pipeline, data);
		list = list->next;
	}
	return (result);
}

int	execute_full_command(t_command *cmd, t_data *data)
{
	int	status;

	status = 1;
	if (!cmd)
		return (status);
	if (cmd->subshell)
		status = execute_subshell(cmd, data);
	else if (cmd->next_pipe)
		status = execute_pipeline(cmd, data);
	else
		status = execute_command(cmd, data);
	return (status);
}

int	execute_command(t_command *cmd, t_data *data)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;
	int	no_args_ret;

	if (!cmd)
		return (1);
	saved_stdin = -1;
	saved_stdout = -1;
	if (!save_original_fds(&saved_stdin, &saved_stdout))
		return (1);
	no_args_ret = handle_no_command_arguments(cmd, saved_stdin, saved_stdout);
	if (no_args_ret != -1)
	{
		data->exit_status = no_args_ret;
		return (no_args_ret);
	}
	status = dispatch_execution(cmd, data);
	restore_original_fds(saved_stdin, saved_stdout);
	data->exit_status = status;
	return (status);
}

int	dispatch_execution(t_command *cmd, t_data *data)
{
	int	status;

	if (is_builtins(cmd->args[0]))
	{
		status = execute_builtin_with_redir(cmd, data);
	}
	else
	{
		status = execute_external_command(cmd, data);
	}
	return (status);
}
