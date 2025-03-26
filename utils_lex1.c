/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lex1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:54:29 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/26 19:33:40 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	handle_whitespace(t_token_state *state)
{
	if (!state->in_single && !state->in_double
		&& is_whitespace(state->input[state->i]))
		return ;
}
