/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lex3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 19:01:24 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/26 19:33:49 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_state	initialize_token_state(const char *input)
{
	t_token_state	state;

	state.input = input;
	state.i = 0;
	state.in_single = false;
	state.in_double = false;
	state.buffer = malloc(sizeof(t_buffer));
	if (!state.buffer)
	{
		perror("malloc");
		exit(1);
	}
	state.buffer->cap = 4096;
	state.buffer->index = 0;
	state.buffer->str = malloc(state.buffer->cap);
	if (!state.buffer->str)
	{
		free(state.buffer);
		perror("malloc");
		exit(1);
	}
	return (state);
}

void	cleanup_token_state(t_token_state *state)
{
	free(state->buffer->str);
	free(state->buffer);
}

void	append_token_to_list(t_token **head, t_token **tail, t_token *new_token)
{
	if (!*head)
	{
		*head = new_token;
		*tail = new_token;
	}
	else
	{
		(*tail)->next = new_token;
		*tail = new_token;
	}
}
