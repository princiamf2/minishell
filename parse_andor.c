/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_andor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:07:51 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:47:19 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	parse_andor_append(t_andor **tail_ptr, t_token **cur)
{
	t_token_type	op;
	t_command		*next_pipeline;
	t_andor			*node;

	op = (*cur)->type;
	*cur = (*cur)->next;
	if (!*cur)
		return (false);
	next_pipeline = parse_pipeline(cur);
	if (!next_pipeline)
		return (false);
	node = init_andor(next_pipeline);
	if (!node)
	{
		free_command(next_pipeline);
		return (false);
	}
	node->op = op;
	(*tail_ptr)->next = node;
	*tail_ptr = node;
	return (true);
}

t_andor	*parse_andor(t_token **cur)
{
	t_command	*first_pipeline;
	t_andor		*head;
	t_andor		*tail;

	head = NULL;
	first_pipeline = parse_pipeline(cur);
	if (!first_pipeline)
		return (NULL);
	head = init_andor(first_pipeline);
	if (!head)
	{
		free_command(first_pipeline);
		return (NULL);
	}
	tail = head;
	while (*cur && ((*cur)->type == AND_IF || (*cur)->type == OR_IF))
	{
		if (!parse_andor_append(&tail, cur))
		{
			free_andor(head);
			return (NULL);
		}
	}
	return (head);
}

void	append_andor(t_andor *head, t_andor *node)
{
	t_andor	*cur;

	cur = head;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

t_andor	*init_andor(t_command *pipeline)
{
	t_andor	*andor;

	andor = malloc(sizeof(t_andor));
	if (!andor)
	{
		perror("malloc");
		return (NULL);
	}
	andor->pipeline = pipeline;
	andor->op = 0;
	andor->next = NULL;
	return (andor);
}
