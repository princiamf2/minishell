/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:19:06 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/27 14:20:45 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (!ptr)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (old_size > 0)
	{
		if (new_size < old_size)
			old_size = new_size;
		ft_memcpy(new_ptr, ptr, old_size);
	}
	free(ptr);
	return (new_ptr);
}
