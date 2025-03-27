/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils10.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:14:29 by michel            #+#    #+#             */
/*   Updated: 2025/03/25 17:35:41 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cmdlist(t_cmdlist *commands, t_data *data)
{
	int			last_status;
	t_cmdlist	*node;

	last_status = 0;
	node = commands;
	while (node)
	{
		last_status = execute_andor(node->andor, data);
		node = node->next;
	}
	return (last_status);
}

int	execute_andor(t_andor *list, t_data *data)
{
	int		result;
	bool	should_run;
	t_andor	*node;

	result = 0;
	should_run = true;
	node = list;
	while (node)
	{
		if (should_run)
			result = execute_full_command(node->pipeline, data);
		if (node->op == AND_IF)
			should_run = (result == 0);
		else if (node->op == OR_IF)
			should_run = (result != 0);
		else
			should_run = true;
		node = node->next;
	}
	return (result);
}

void	free_cmdlist(t_cmdlist *list)
{
	t_cmdlist	*next;

	while (list)
	{
		next = list->next;
		free_andor(list->andor);
		free(list);
		list = next;
	}
}

void	free_andor(t_andor *andor)
{
	t_andor	*next;

	while (andor)
	{
		next = andor->next;
		free_command(andor->pipeline);
		free(andor);
		andor = next;
	}
}

int	execute_builtin_with_redir(t_command *cmd, t_data *data)
{
	int	saved_stdout;
	int	ret;

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout < 0)
	{
		perror("dup");
		return (1);
	}
	if (handle_redirection(cmd) < 0)
	{
		close(saved_stdout);
		return (1);
	}
	ret = execute_builtin(cmd->args, data);
	if (dup2(saved_stdout, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		close(saved_stdout);
		return (1);
	}
	close(saved_stdout);
	return (ret);
}
