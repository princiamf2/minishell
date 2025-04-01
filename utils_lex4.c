/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lex4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 16:47:55 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/31 17:36:37 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_in_redirect(t_token_state *state)
{
	if (state->input[state->i] != '<')
		return (false);
	if (state->input[state->i + 1] != '\0' && state->input[state->i + 1] == '<')
	{
		if (state->buffer->index != 0)
			return (true);
		append_to_buffer(state->buffer, "<<");
		state->i += 2;
		return (true);
	}
	else
	{
		if (state->buffer->index != 0)
			return (true);
		append_to_buffer(state->buffer, "<");
		state->i++;
		return (true);
	}
}

bool	handle_out_redirect(t_token_state *state)
{
	if (state->input[state->i] != '>')
		return (false);
	if (state->input[state->i + 1] != '\0' && state->input[state->i + 1] == '>')
	{
		if (state->buffer->index != 0)
			return (true);
		append_to_buffer(state->buffer, ">>");
		state->i += 2;
		return (true);
	}
	else
	{
		if (state->buffer->index != 0)
			return (true);
		append_to_buffer(state->buffer, ">");
		state->i++;
		return (true);
	}
}

bool	handle_pipe_operator(t_token_state *state)
{
	if (state->input[state->i] != '|')
		return (false);
	if (state->input[state->i + 1] != '\0' && state->input[state->i + 1] == '|')
	{
		if (state->buffer->index != 0)
			return (true);
		append_to_buffer(state->buffer, "||");
		printf("Opérateur || détecté\n");
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
		printf("Opérateur && détecté\n");
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

bool	handle_special_operators(t_token_state *state)
{
	if (state->in_single || state->in_double)
		return (false);
	if (handle_in_redirect(state))
		return (true);
	if (handle_out_redirect(state))
		return (true);
	if (handle_pipe_operator(state))
		return (true);
	if (handle_amp_operator(state))
		return (true);
	return (false);
}
