/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lex2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:17:42 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/25 16:57:20 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(const char *str)
{
	t_token	*token;

	token = allocate_token();
	if (!token)
		return (NULL);
	initialize_token(token, str);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	return (token);
}

char	*collect_token(t_token_state *state, int exit_status, t_env *env)
{
	state->buffer->index = 0;
	while (state->input[state->i])
	{
		if (!state->in_single && !state->in_double
			&& is_whitespace(state->input[state->i]))
			break ;
		if (!state->in_single && !state->in_double
			&& (state->input[state->i] == '(' || state->input[state->i] == ')'))
			break ;
		if (handle_special_operators(state))
			break ;
		process_token_char(state, exit_status, env);
	}
	state->buffer->str[state->buffer->index] = '\0';
	return (ft_strdup(state->buffer->str));
}

void	handle_whitespace(t_token_state *state)
{
	if (!state->in_single && !state->in_double
		&& is_whitespace(state->input[state->i]))
		return ;
}

void	handle_quotes(t_token_state *state)
{
	char	c;

	c = state->input[state->i];
	if (c == '\'' && !state->in_single)
	{
		state->in_single = !state->in_single;
		state->i++;
		return ;
	}
	else if (c == '\"' && !state->in_double)
	{
		state->in_double = !state->in_double;
		state->i++;
		return ;
	}
}

void	handle_escape_character(t_token_state *state)
{
	if (state->input[state->i] == '\\' && !state->in_single)
	{
		state->i++;
		if (!state->input[state->i])
			return ;
	}
}
