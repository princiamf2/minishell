/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell_collect.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:34:47 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:54:23 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*finalize_subshell_collection(t_token *head, t_token **cur,
		t_token *current_original, int balance)
{
	if (balance != 0)
	{
		ft_putendl_fd("minishell: syntax error: unclosed parenthesis", 2);
		free_tokens(head);
		return (NULL);
	}
	if (current_original)
		*cur = current_original->next;
	else
		*cur = NULL;
	return (head);
}

t_token	*process_subshell_tokens_loop(t_token **cur, int *balance_ptr,
		t_token **head_ptr, t_token **tail_ptr)
{
	t_token	*current_original;
	t_token	*extracted_token;

	current_original = *cur;
	while (current_original)
	{
		if (should_break_subshell_loop(current_original, balance_ptr))
			break ;
		extracted_token = duplicate_original_token(current_original);
		if (!extracted_token)
		{
			free_tokens(*head_ptr);
			return (NULL);
		}
		append_token_to_list(head_ptr, tail_ptr, extracted_token);
		current_original = current_original->next;
	}
	return (current_original);
}

t_token	*collect_tokens_until_closing(t_token **cur)
{
	t_token	*head;
	t_token	*tail;
	t_token	*last_processed;
	int		balance;

	head = NULL;
	tail = NULL;
	if (!cur || !*cur)
		return (NULL);
	balance = 1;
	last_processed = process_subshell_tokens_loop(cur, &balance, &head, &tail);
	if (!head && balance == 1 && !last_processed)
	{
		return (NULL);
	}
	return (finalize_subshell_collection(head, cur, last_processed, balance));
}

bool	should_break_subshell_loop(t_token *token, int *balance)
{
	if (token->type == PAREN_OPEN)
	{
		(*balance)++;
	}
	else if (token->type == PAREN_CLOSE)
	{
		(*balance)--;
		if (*balance == 0)
			return (true);
	}
	if (*balance < 0)
		return (true);
	return (false);
}
