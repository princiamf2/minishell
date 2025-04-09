/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lex2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:17:42 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/07 13:39:51 by nicolsan         ###   ########.fr       */
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

bool	is_paren(char c)
{
	return (c == '(' || c == ')');
}

char	*check_initial_paren(t_token_state *state)
{
	char	paren_str[2];

	if (!state->in_single && !state->in_double
		&& is_paren(state->input[state->i]))
	{
		paren_str[0] = state->input[state->i];
		paren_str[1] = '\0';
		append_to_buffer(state->buffer, paren_str);
		state->i++;
		state->buffer->str[state->buffer->index] = '\0';
		return (ft_strdup(state->buffer->str));
	}
	return (NULL);
}

bool	should_break_collecting(t_token_state *state)
{
	if (!state->in_single && !state->in_double)
	{
		if (is_whitespace(state->input[state->i]))
			return (true);
		if (is_paren(state->input[state->i]))
			return (true);
		if (handle_special_operators(state))
		{
			if (state->buffer->index == 0)
				return (false);
			else
				return (true);
		}
	}
	return (false);
}

char	*finalize_collected_token(t_token_state *state)
{
	state->buffer->str[state->buffer->index] = '\0';
	if (state->buffer->index == 0 && state->input[state->i] == '\0')
		return (NULL);
	return (ft_strdup(state->buffer->str));
}

char	*collect_token(t_token_state *state, t_data *data, t_env *env)
{
	char	*initial_token;

	state->buffer->index = 0;
	initial_token = check_initial_paren(state);
	if (initial_token)
		return (initial_token);
	while (state->input[state->i])
	{
		if (should_break_collecting(state))
		{
			if (state->buffer->index == 0)
				continue ;
			else
				break ;
		}
		process_token_char(state, data->exit_status, env);
	}
	return (finalize_collected_token(state));
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
	if (c == '\'' && !state->in_double)
	{
		state->in_single = !state->in_single;
		state->i++;
		return ;
	}
	else if (c == '\"' && !state->in_single)
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
