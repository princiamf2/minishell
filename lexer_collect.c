/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_collect.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:17:42 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:47:40 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	char	*result;

	state->buffer->str[state->buffer->index] = '\0';
	if (state->buffer->index == 0 && state->input
		&& state->input[state->i] == '\0')
	{
		result = ft_strdup("");
		if (!result)
			perror("minishell: ft_strdup (finalize empty)");
		return (result);
	}
	if (state->buffer->index == 0)
	{
		result = ft_strdup("");
		if (!result)
			perror("minishell: ft_strdup (finalize empty 2)");
		return (result);
	}
	result = ft_strdup(state->buffer->str);
	if (!result)
		perror("minishell: ft_strdup (finalize)");
	return (result);
}

char	*collect_token(t_token_state *state, t_data *data)
{
	char	*initial_token;
	size_t	start_index;

	start_index = state->i;
	state->buffer->index = 0;
	state->buffer->str[0] = '\0';
	initial_token = check_initial_paren(state);
	if (initial_token)
		return (initial_token);
	if (handle_special_operators(state))
	{
		if (state->buffer->index > 0)
			return (finalize_collected_token(state));
	}
	state->buffer->index = 0;
	state->buffer->str[0] = '\0';
	state->i = start_index;
	while (state->input && state->input[state->i])
	{
		if (should_break_collecting(state))
			break ;
		process_token_char(state, data->exit_status, data);
	}
	return (finalize_collected_token(state));
}
