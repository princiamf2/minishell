/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:47:37 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 14:02:58 by nicolsan         ###   ########.fr       */
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
	if (!node->andor)
	{
		free(node);
		return (NULL);
	}
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

bool	validate_tok_aft_andor(t_token **current_token_ptr_ptr)
{
	if (*current_token_ptr_ptr && (*current_token_ptr_ptr)->type == SEMICOLON)
	{
		*current_token_ptr_ptr = (*current_token_ptr_ptr)->next;
		if (*current_token_ptr_ptr
			&& (*current_token_ptr_ptr)->type == SEMICOLON)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `;;'",
				2);
			return (false);
		}
	}
	else if (*current_token_ptr_ptr)
	{
		ft_putendl_fd("minishell: syntax error", 2);
		return (false);
	}
	return (true);
}

bool	parse_and_append_cmdlist_node(t_cmdlist **head_ptr,
		t_cmdlist **tail_ptr, t_token **current_token_ptr_ptr)
{
	t_cmdlist	*node;

	node = create_cmdlist_node(current_token_ptr_ptr);
	if (!node)
	{
		return (false);
	}
	append_node(head_ptr, tail_ptr, node);
	if (!validate_tok_aft_andor(current_token_ptr_ptr))
	{
		return (false);
	}
	return (true);
}

t_cmdlist	*parse_line(t_token *tokens)
{
	t_token		*current_token_ptr;
	t_cmdlist	*head;
	t_cmdlist	*tail;

	head = NULL;
	tail = NULL;
	current_token_ptr = tokens;
	if (!current_token_ptr)
		return (NULL);
	while (current_token_ptr)
	{
		if (!parse_and_append_cmdlist_node(&head, &tail, &current_token_ptr))
		{
			free_cmdlist(head);
			return (NULL);
		}
	}
	return (head);
}
