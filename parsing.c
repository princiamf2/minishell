/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:47:37 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/25 15:22:22 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmdlist	*create_cmdlist_node(t_token **tokens)
{
	t_cmdlist	*node;

	node = malloc(sizeof(t_cmdlist));
	if (!node)
	{
		perror("malloc");
		return (NULL);
	}
	node->andor = parse_andor(tokens);
	node->next = NULL;
	return (node);
}

void	append_node(t_cmdlist **head, t_cmdlist **tail, t_cmdlist *node)
{
	if (*head == NULL)
	{
		*head = node;
		*tail = node;
	}
	else
	{
		(*tail)->next = node;
		*tail = node;
	}
}

t_cmdlist	*parse_line(t_token *tokens)
{
	t_cmdlist	*head;
	t_cmdlist	*tail;
	t_cmdlist	*node;

	if (!tokens)
		return (NULL);
	head = NULL;
	tail = NULL;
	while (tokens)
	{
		node = create_cmdlist_node(&tokens);
		if (!node)
			break ;
		append_node(&head, &tail, node);
		skip_semicolon(&tokens);
	}
	return (head);
}

t_command	*parse_command(t_token **cur)
{
	t_command	*cmd;
	t_command	*sub;
	int			argc;

	init_command();
	if (*cur && ft_strcmp((*cur)->value, "(") == 0)
	{
		sub = parse_subshell(cur);
		return (sub);
	}
	cmd = init_command();
	if (!cmd)
		return (NULL);
	argc = parse_command_arguments(cmd, cur);
	if (argc < 0)
		return (NULL);
	return (cmd);
}

t_command	*parse_subshell(t_token **cur)
{
	t_token		*sub_tokens;
	t_cmdlist	*sub_ast;
	t_command	*cmd;

	sub_tokens = extract_subshell_tokens(cur);
	if (!sub_tokens)
		return (NULL);
	sub_ast = build_subshell_ast(sub_tokens);
	*cur = (*cur)->next;
	cmd = create_subshell_command(sub_ast);
	if (!cmd)
		return (NULL);
	return (cmd);
}
