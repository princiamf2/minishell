/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lex1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:16:19 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/07 11:17:52 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*remove_quotes(const char *str)
{
	size_t	i;
	size_t	j;
	char	*new_str;

	new_str = malloc(ft_strlen(str) + 1);
	i = 0;
	j = 0;
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

void	initialize_token(t_token *token, const char *str)
{
	char	*raw;

	raw = duplicate_token_value(str);
	if (!raw)
	{
		token->value = NULL;
		return ;
	}
	token->value = remove_quotes(raw);
	free(raw);
	token->type = determine_token_type(str);
	token->quoted = false;
	token->next = NULL;
}
