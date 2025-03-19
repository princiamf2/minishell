/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:06:56 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/19 16:08:22 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*allocate_token(void)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		perror("malloc");
	return (token);
}

char	*duplicate_token_value(const char *str)
{
	char	*dup;

	dup = strdup(str);
	if (!dup)
		perror("strdup");
	return (dup);
}

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

char *remove_quotes(const char *str)
{
	size_t i;
	size_t j;
	char *new_str;

	new_str = malloc(ft_strlen(str) + 1);
	i = 0;
	j = 0;
	if (!new_str)
		return (NULL);
	while (str[i])
	{
		if (str[i] != '\'' && str[i] != '\"')
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}

void	initialize_token(t_token *token, const char *str)
{
	char *raw;

	raw = duplicate_token_value(str);
	if (!raw)
	{
		token->value = NULL;
		return ;
	}
	token->value = remove_quotes(raw);
	free(raw);
	token->type = determine_token_type(str);
	token->quoted = false;
	token->next = NULL;
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

char *collect_token(t_token_state *state, int exit_status, t_env *env)
{
	state->buffer->index = 0;
	while (state->input[state->i])
	{
		if (!state->in_single && !state->in_double && is_whitespace(state->input[state->i]))
			break;
		if (!state->in_single && !state->in_double &&
			(state->input[state->i] == '(' || state->input[state->i] == ')'))
			break;
		process_token_char(state, exit_status, env);
	}
	state->buffer->str[state->buffer->index] = '\0';
	return (ft_strdup(state->buffer->str));
}

void	handle_whitespace(t_token_state *state)
{
	if (!state->in_single && !state->in_double && is_whitespace(state->input[state->i]))
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

void	handle_dollar_sign(t_token_state *state, int exit_status, t_env *env)
{
	if (state->input[state->i] == '$' && !state->in_single)
	{
		if (state->input[state->i + 1] == '?')
			handle_dollar_question(state->input, &state->i, state->buffer, exit_status);
		else
			handle_dollar_variable(state->input, &state->i, state->buffer, env);
	}
}

void	process_regular_character(t_token_state *state)
{
	char	c;

	c = state->input[state->i];
	append_to_buffer(state->buffer, (char[]){c, '\0'});
	state->i++;
}

void	process_token_char(t_token_state *state, int exit_status, t_env *env)
{

	handle_whitespace(state);
	handle_quotes(state);
	handle_escape_character(state);
	handle_dollar_sign(state, exit_status, env);
	process_regular_character(state);
}

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

void append_token_to_list(t_token **head, t_token **tail, t_token *new_token)
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

t_token	*process_tokens(t_token_state *state)
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
		token = create_token(collect_token(state, 0, NULL));
		if (!token)
			break ;
		append_token_to_list(&head, &tail, token);
		i = skip_whitespace(state->input, state->i);
	}
	return (head);
}

t_token	*lexer(const char *input)
{
	t_token_state	state;
	t_token	*head;

	state = initialize_token_state(input);
	head = process_tokens(&state);
	cleanup_token_state(&state);
	return (head);
}
