/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:34:08 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:35:33 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command(t_command *cmd)
{
	t_command	*next_pipe_to_free;

	if (!cmd)
		return ;
	next_pipe_to_free = cmd->next_pipe;
	if (!cmd->subshell)
		free_command_args(cmd);
	free_command_redirs(cmd);
	if (cmd->input)
		free(cmd->input);
	if (cmd->output)
		free(cmd->output);
	if (cmd->subshell && cmd->subshell_ast)
		free_cmdlist(cmd->subshell_ast);
	free(cmd);
	if (next_pipe_to_free)
		free_command(next_pipe_to_free);
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

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}
