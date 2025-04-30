/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_op_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:16:43 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:47:25 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_pipe_operator(t_token_state *state)
{
	if (state->input[state->i] != '|')
		return (false);
	if (state->input[state->i + 1] != '\0' && state->input[state->i + 1] == '|')
	{
		if (state->buffer->index != 0)
			return (true);
		append_to_buffer(state->buffer, "||");
		state->i += 2;
		return (true);
	}
	else
	{
		if (state->buffer->index != 0)
			return (true);
		append_to_buffer(state->buffer, "|");
		state->i++;
		return (true);
	}
}

bool	handle_amp_operator(t_token_state *state)
{
	if (state->input[state->i] != '&')
		return (false);
	if (state->input[state->i + 1] != '\0' && state->input[state->i + 1] == '&')
	{
		if (state->buffer->index != 0)
			return (true);
		append_to_buffer(state->buffer, "&&");
		state->i += 2;
		return (true);
	}
	else
	{
		if (state->buffer->index != 0)
			return (true);
		append_to_buffer(state->buffer, "&");
		state->i++;
		return (true);
	}
}

bool	handle_semicolon_operator(t_token_state *state)
{
	if (state->input[state->i] == ';')
	{
		if (state->buffer->index != 0)
			return (true);
		else
		{
			append_to_buffer(state->buffer, ";");
			state->i++;
			return (true);
		}
	}
	return (false);
}
