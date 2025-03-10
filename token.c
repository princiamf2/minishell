/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 18:19:06 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/10 18:19:21 by mm-furi          ###   ########.fr       */
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
		if (strcmp(cur->value, "(") == 0)
			paren_balance++;
		else if (strcmp(cur->value, ")") == 0)
		{
			paren_balance--;
			if (paren_balance < 0)
			{
				fprintf(stderr, "[erreur] Parenthèses desequilibrees : trop de ')'\n");
				return (false);
			}
		}
		if (cur->type == REDIR_IN || cur->type == REDIR_OUT ||
			cur->type == REDIR_APPEND || cur->type == HEREDOC)
			{
			if (!cur->next || cur->next->type != WORD)
			{
				fprintf(stderr, "[erreur] Redirection sans cible : \"%s\"\n", cur->value);
				return (false);
			}
		}
		if (cur->type == AND_IF || cur->type == OR_IF)
		{
			if (prev_was_operator)
			{
				fprintf(stderr, "[erreur] Deux operateurs logiques consecutifs\n");
				return (false);
			}
			prev_was_operator = true;
		}
		else if (cur->type != SEMICOLON)
		{
			prev_was_operator = false;
		}
		cur = cur->next;
	}
	if (paren_balance != 0)
	{
		fprintf(stderr, "[erreur] Parenthèses desequilibrees : balance = %d\n", paren_balance);
		return (false);
	}
	return (true);
}
