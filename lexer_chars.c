/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_chars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:19:44 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 14:39:13 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_token_char(t_token_state *state, int exit_status, t_data *data)
{
	char	current_char;

	if (!state->input || !state->input[state->i])
		return ;
	current_char = state->input[state->i];
	if ((current_char == '\'' && !state->in_double) || (current_char == '\"'
			&& !state->in_single))
	{
		handle_quotes(state);
		return ;
	}
	if (current_char == '\\' && !state->in_single)
	{
		handle_escape_character(state);
		return ;
	}
	if (current_char == '$' && !state->in_single)
	{
		handle_dollar_sign(state, exit_status, data);
		return ;
	}
	append_char_as_string(state->buffer, current_char);
	state->i++;
}

void	process_regular_character(t_token_state *state)
{
	char	c;

	if (!state->input[state->i])
		return ;
	c = state->input[state->i];
	append_char_as_string(state->buffer, c);
	state->i++;
}

void	handle_whitespace(t_token_state *state)
{
	if (!state->input || !state->input[state->i])
		return ;
	if (!state->in_single && !state->in_double
		&& is_whitespace(state->input[state->i]))
	{
		return ;
	}
}

void	handle_quotes(t_token_state *state)
{
	char	c;

	if (!state->input || !state->input[state->i])
		return ;
	c = state->input[state->i];
	append_char_as_string(state->buffer, c);
	state->i++;
	if (c == '\'' && !state->in_double)
	{
		state->in_single = !state->in_single;
	}
	else if (c == '\"' && !state->in_single)
	{
		state->in_double = !state->in_double;
	}
}

void	handle_escape_character(t_token_state *state)
{
	if (!state->input || !state->input[state->i])
		return ;
	append_to_buffer(state->buffer, "\\");
	state->i++;
	if (state->input[state->i])
	{
		append_char_as_string(state->buffer, state->input[state->i]);
		state->i++;
	}
}
