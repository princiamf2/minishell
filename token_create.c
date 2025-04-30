/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:16:19 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:49:20 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(const char *raw_value)
{
	t_token	*token;

	if (!raw_value)
		return (NULL);
	token = allocate_token();
	if (!token)
		return (NULL);
	initialize_token(token, raw_value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	return (token);
}

t_token	*allocate_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		perror("malloc");
	return (token);
}

char	*duplicate_token_value(const char *str)
{
	char	*dup;

	dup = strdup(str);
	if (!dup)
		perror("strdup");
	return (dup);
}

t_token_type	determine_token_type(const char *str)
{
	if (strcmp(str, "|") == 0)
		return (PIPE);
	if (strcmp(str, "&&") == 0)
		return (AND_IF);
	if (strcmp(str, "||") == 0)
		return (OR_IF);
	if (strcmp(str, ";") == 0)
		return (SEMICOLON);
	if (strcmp(str, ">") == 0)
		return (REDIR_OUT);
	if (strcmp(str, "<") == 0)
		return (REDIR_IN);
	if (strcmp(str, ">>") == 0)
		return (REDIR_APPEND);
	if (strcmp(str, "<<") == 0)
		return (HEREDOC);
	if (strcmp(str, "(") == 0)
		return (PAREN_OPEN);
	if (strcmp(str, ")") == 0)
		return (PAREN_CLOSE);
	return (WORD);
}

void	initialize_token(t_token *token, const char *raw_value)
{
	size_t	len;

	token->value = duplicate_token_value(raw_value);
	if (!token->value)
	{
		return ;
	}
	token->type = determine_token_type(token->value);
	token->quoted = false;
	len = ft_strlen(token->value);
	if (len >= 2)
	{
		if ((token->value[0] == '\'' && token->value[len - 1] == '\'')
			|| (token->value[0] == '\"' && token->value[len - 1] == '\"'))
		{
			token->quoted = true;
		}
	}
	token->next = NULL;
}
