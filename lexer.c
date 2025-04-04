/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:06:56 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/04 13:41:16 by mm-furi          ###   ########.fr       */
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

t_token	*process_tokens(t_token_state *state, t_env *env, t_data *data)
{
	t_token	*head;
	t_token	*tail;
	t_token	*token;
	size_t	i;

	i = skip_whitespace(state->input, 0);
	head = NULL;
	tail = NULL;
	while (state->input[i])
	{
		state->i = i;
		state->buffer->index = 0;
		token = create_token(collect_token(state, data, env));
		if (!token)
			break ;
		append_token_to_list(&head, &tail, token);
		i = skip_whitespace(state->input, state->i);
	}
	return (head);
}

t_token	*lexer(const char *input, t_env *env, t_data *data)
{
	t_token_state	state;
	t_token			*head;

	state = initialize_token_state(input);
	head = process_tokens(&state, env, data);
	cleanup_token_state(&state);
	expand_tokens(head, env);
	return (head);
}
