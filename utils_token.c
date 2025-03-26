/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:23:46 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/25 15:29:29 by mm-furi          ###   ########.fr       */
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

bool	is_operator(t_token *token)
{
	return (token->type == AND_IF || token->type == OR_IF
		|| token->type == PIPE);
}

bool	adjacent_operators(t_token *prev, t_token *cur)
{
	return (is_operator(prev) && is_operator(cur));
}
