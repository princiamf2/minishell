/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:28:25 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:49:32 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	skip_opening_paren(t_token **cur)
{
	if (!cur || !*cur || (*cur)->type != PAREN_OPEN)
		return (false);
	*cur = (*cur)->next;
	return (true);
}

t_token	*duplicate_original_token(t_token *original)
{
	t_token	*extracted_token;

	extracted_token = allocate_token();
	if (!extracted_token)
		return (NULL);
	initialize_token(extracted_token, original->value);
	if (!extracted_token->value && original->value)
	{
		free(extracted_token);
		return (NULL);
	}
	extracted_token->quoted = original->quoted;
	extracted_token->next = NULL;
	return (extracted_token);
}
