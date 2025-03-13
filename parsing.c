/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:47:37 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/13 14:27:12 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmdlist *parse_line(t_token *tokens)
{
	t_cmdlist *head;
	t_cmdlist *tail;
	t_cmdlist *node;

	fprintf(stderr, "[debuger] Entree dans parse_line()\n");
	if (!tokens)
	{
		fprintf(stderr, "[debuger] Aucun token a parser\n");
		return NULL;
	}
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
	fprintf(stderr, "[debuger] Fin de parse_line()\n");
	return head;
}

t_command *parse_command(t_token **cur)
{
	t_command *cmd;
	t_command *sub;
	int argc;

	fprintf(stderr, "[debuger] Entree dans parse_command()\n");
	init_command();
	if (*cur && ft_strcmp((*cur)->value, "(") == 0)
	{
		sub = parse_subshell(cur);
		fprintf(stderr, "[debuger] Fin de parse_command() avec subshell\n");
		return sub;
	}
	cmd = init_command();
	if (!cmd)
		return NULL;
	argc = parse_command_arguments(cmd, cur);
	if (argc < 0)
		return NULL;
	fprintf(stderr, "[debuger] Fin de parse_command() avec %d argument(s)\n", argc);
	return cmd;
}

t_command *parse_subshell(t_token **cur)
{
	t_token *sub_tokens;
	t_cmdlist *sub_ast;
	t_command *cmd;

	fprintf(stderr, "[debuger] Entree dans parse_subshell()\n");
	sub_tokens = extract_subshell_tokens(cur);
	if (!sub_tokens)
		return NULL;
	sub_ast = build_subshell_ast(sub_tokens);
	*cur = (*cur)->next;
	cmd = create_subshell_command(sub_ast);
	if (!cmd)
		return NULL;
	fprintf(stderr, "[debuger] Fin de parse_subshell()\n");
	return cmd;
}
