/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:19:06 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/18 17:12:51 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool validate_tokens(t_token *tokens)
{
	int paren_balance = 0;
	t_token *cur = tokens;
	bool prev_was_operator = false;
	while (cur)
	{
		if (ft_strcmp(cur->value, "(") == 0)
			paren_balance++;
		else if (ft_strcmp(cur->value, ")") == 0)
		{
			paren_balance--;
			if (paren_balance < 0)
				return (false);
		}
		if (cur->type == REDIR_IN || cur->type == REDIR_OUT ||
			cur->type == REDIR_APPEND || cur->type == HEREDOC)
			{
			if (!cur->next || cur->next->type != WORD)
				return (false);
		}
		if (cur->type == AND_IF || cur->type == OR_IF)
		{
			if (prev_was_operator)
				return (false);
			prev_was_operator = true;
		}
		else if (cur->type != SEMICOLON)
		{
			prev_was_operator = false;
		}
		cur = cur->next;
	}
	if (paren_balance != 0)
		return (false);
	return (true);
}

bool validate_tokens_adv(t_token *tokens)
{
	t_token *prev;
	t_token *cur;
	if (!tokens)
		return (false);
	if (tokens->type == AND_IF || tokens->type == OR_IF || tokens->type == PIPE)
		return (false);
	prev = tokens;
	cur = tokens->next;
	while (cur)
	{
		if ((cur->type == AND_IF || cur->type == OR_IF || cur->type == PIPE) &&
			(prev->type == AND_IF || prev->type == OR_IF || prev->type == PIPE))
			return (false);
		if (cur->type == REDIR_IN || cur->type == REDIR_OUT ||
			cur->type == REDIR_APPEND || cur->type == HEREDOC)
		{
			if (!cur->next || cur->next->type != WORD)
				return (false);
		}
		prev = cur;
		cur = cur->next;
	}
	if (prev->type == AND_IF || prev->type == OR_IF || prev->type == PIPE)
		return (false);

	return (true);
}
