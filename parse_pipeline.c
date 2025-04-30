/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipeline.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 20:05:19 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:49:42 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	link_pipeline_commands(t_command **current_ptr, t_token **cur,
		t_command *first_for_cleanup)
{
	t_command	*next_cmd;

	while (*cur && ((*cur)->type == PIPE))
	{
		*cur = (*cur)->next;
		if (!*cur)
		{
			free_command(first_for_cleanup);
			return (false);
		}
		next_cmd = parse_command(cur);
		if (!next_cmd)
		{
			free_command(first_for_cleanup);
			return (false);
		}
		(*current_ptr)->next_pipe = next_cmd;
		*current_ptr = next_cmd;
	}
	return (true);
}

t_command	*parse_pipeline(t_token **cur)
{
	t_command	*first;
	t_command	*current;

	first = parse_command(cur);
	if (!first)
		return (NULL);
	current = first;
	if (!link_pipeline_commands(&current, cur, first))
	{
		return (NULL);
	}
	return (first);
}
