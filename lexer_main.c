/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:06:56 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:47:39 by nicolsan         ###   ########.fr       */
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

bool	process_token_loop_step(t_token_list *list, t_token_state *state,
		t_data *data, size_t *i_ptr)
{
	char	*raw_value;
	t_token	*new_token;
	size_t	token_start_index;

	token_start_index = *i_ptr;
	state->i = *i_ptr;
	state->buffer->index = 0;
	raw_value = collect_token(state, data);
	if (!raw_value)
		return (false);
	if (handle_empty_raw_token(raw_value, state, i_ptr))
		return (true);
	new_token = create_token(raw_value);
	if (!new_token)
	{
		free(raw_value);
		return (false);
	}
	qualify_token_quoting(new_token, state, token_start_index);
	append_token_to_list(&(list->head), &(list->tail), new_token);
	free(raw_value);
	*i_ptr = skip_whitespace(state->input, state->i);
	return (true);
}

t_token	*process_tokens(t_token_state *state, t_data *data)
{
	t_token_list	list;
	size_t			i;

	i = skip_whitespace(state->input, 0);
	list.head = NULL;
	list.tail = NULL;
	while (state->input[i])
	{
		if (!process_token_loop_step(&list, state, data, &i))
		{
			free_tokens(list.head);
			return (NULL);
		}
	}
	return (list.head);
}

t_token	*lexer(const char *input, t_env *env, t_data *data)
{
	t_token_state	state;
	t_token			*head;

	state = initialize_token_state(input);
	head = process_tokens(&state, data);
	cleanup_token_state(&state);
	expand_tokens(head, env, data->local_vars);
	return (head);
}
