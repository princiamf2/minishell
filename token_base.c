/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_base.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:23:46 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 14:14:00 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	update_paren(t_token *cur, int *paren_balance)
{
	if (ft_strcmp(cur->value, "(") == 0)
	{
		(*paren_balance)++;
	}
	else if (ft_strcmp(cur->value, ")") == 0)
	{
		(*paren_balance)--;
		if (*paren_balance < 0)
			return (false);
	}
	return (true);
}

bool	validate_redirection(t_token *cur)
{
	if (cur->type == REDIR_IN || cur->type == REDIR_OUT
		|| cur->type == REDIR_APPEND || cur->type == HEREDOC)
	{
		if (!cur->next || cur->next->type != WORD)
			return (false);
	}
	return (true);
}

bool	update_operator(t_token *cur, bool *prev_operator)
{
	if (cur->type == AND_IF || cur->type == OR_IF)
	{
		if (*prev_operator)
			return (false);
		*prev_operator = true;
	}
	else if (cur->type != SEMICOLON)
	{
		*prev_operator = false;
	}
	return (true);
}

bool	validate_tokens(t_token *tokens)
{
	int		paren_balance;
	t_token	*cur;
	bool	prev_was_operator;

	paren_balance = 0;
	cur = tokens;
	prev_was_operator = false;
	while (cur)
	{
		if (!update_paren(cur, &paren_balance))
			return (false);
		if (!validate_redirection(cur))
			return (false);
		if (!update_operator(cur, &prev_was_operator))
			return (false);
		cur = cur->next;
	}
	return (paren_balance == 0);
}
