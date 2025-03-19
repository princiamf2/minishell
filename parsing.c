/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:47:37 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/18 17:14:13 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmdlist *parse_line(t_token *tokens)
{
	t_cmdlist *head;
	t_cmdlist *tail;
	t_cmdlist *node;

	if (!tokens)
		return NULL;
	head = NULL;
	tail = NULL;
	while (tokens)
	{
		node = malloc(sizeof(t_cmdlist));
		if (!node)
		{
			perror("malloc");
			break;
		}
		node->andor = parse_andor(&tokens);
		node->next = NULL;
		if (!head)
			head = tail = node;
		else
		{
			tail->next = node;
			tail = node;
		}
		if (tokens && tokens->type == SEMICOLON)
			tokens = tokens->next;
	}
	return head;
}

t_command *parse_command(t_token **cur)
{
	t_command *cmd;
	t_command *sub;
	int argc;

	init_command();
	if (*cur && ft_strcmp((*cur)->value, "(") == 0)
	{
		sub = parse_subshell(cur);
		return sub;
	}
	cmd = init_command();
	if (!cmd)
		return NULL;
	argc = parse_command_arguments(cmd, cur);
	if (argc < 0)
		return NULL;
	return cmd;
}

t_command *parse_subshell(t_token **cur)
{
	t_token *sub_tokens;
	t_cmdlist *sub_ast;
	t_command *cmd;

	sub_tokens = extract_subshell_tokens(cur);
	if (!sub_tokens)
		return NULL;
	sub_ast = build_subshell_ast(sub_tokens);
	*cur = (*cur)->next;
	cmd = create_subshell_command(sub_ast);
	if (!cmd)
		return NULL;
	return cmd;
}
