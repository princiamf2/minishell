/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:06:56 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/26 19:32:49 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token_type	determine_token_type(const char *str)
{
	if (strcmp(str, "|") == 0)
		return (PIPE);
	if (strcmp(str, "&&") == 0)
		return (AND_IF);
	if (strcmp(str, "||") == 0)
		return (OR_IF);
	if (strcmp(str, ";") == 0)
		return (SEMICOLON);
	if (strcmp(str, ">") == 0)
		return (REDIR_OUT);
	if (strcmp(str, "<") == 0)
		return (REDIR_IN);
	if (strcmp(str, ">>") == 0)
		return (REDIR_APPEND);
	if (strcmp(str, "<<") == 0)
		return (HEREDOC);
	return (WORD);
}

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

t_token	*process_tokens(t_token_state *state, t_env *env)
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
		token = create_token(collect_token(state, 0, env));
		if (!token)
			break ;
		append_token_to_list(&head, &tail, token);
		i = skip_whitespace(state->input, state->i);
	}
	return (head);
}

t_token	*lexer(const char *input, t_env *env)
{
	t_token_state	state;
	t_token			*head;

	state = initialize_token_state(input);
	head = process_tokens(&state, env);
	cleanup_token_state(&state);
	return (head);
}
