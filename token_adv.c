/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_adv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:19:06 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:52:52 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_redirection(t_token *cur)
{
	if (cur->type == REDIR_IN || cur->type == REDIR_OUT
		|| cur->type == REDIR_APPEND || cur->type == HEREDOC)
	{
		if (!cur->next || cur->next->type != WORD)
			return (false);
	}
	return (true);
}

bool	validate_tokens_adv(t_token *tokens)
{
	t_token	*prev;
	t_token	*cur;

	if (!tokens)
		return (false);
	if (is_operator(tokens))
		return (false);
	prev = tokens;
	cur = tokens->next;
	while (cur)
	{
		if (adjacent_operators(prev, cur))
			return (false);
		if (!check_redirection(cur))
			return (false);
		prev = cur;
		cur = cur->next;
	}
	if (is_operator(prev))
		return (false);
	return (true);
}

bool	is_operator(t_token *token)
{
	return (token->type == AND_IF || token->type == OR_IF
		|| token->type == PIPE);
}

bool	adjacent_operators(t_token *prev, t_token *cur)
{
	return (is_operator(prev) && is_operator(cur));
}
