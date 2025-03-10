/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:06:56 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/10 17:44:58 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token *create_token(const char *str)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
	{
		perror("malloc");
		return NULL;
	}
	token->value = strdup(str);
	if (!token->value)
	{
		free(token);
		return NULL;
	}
	if (strcmp(str, "|") == 0)
		token->type = PIPE;
	else if (strcmp(str, "&&") == 0)
		token->type = AND_IF;
	else if (strcmp(str, "||") == 0)
		token->type = OR_IF;
	else if (strcmp(str, ";") == 0)
		token->type = SEMICOLON;
	else if (strcmp(str, ">") == 0)
		token->type = REDIR_OUT;
	else if (strcmp(str, "<") == 0)
		token->type = REDIR_IN;
	else if (strcmp(str, ">>") == 0)
		token->type = REDIR_APPEND;
	else if (strcmp(str, "<<") == 0)
		token->type = HEREDOC;
	else
		token->type = WORD;
	token->quoted = false;
	token->next = NULL;
	return token;
}

char *collect_token(t_token_state *state)
{
	char buf[4096];
	size_t idx;

	state->in_single = false;
	state->in_double = false;
	if (!state->in_single && !state->in_double && (state->input[state->i] == '(' || state->input[state->i] == ')'))
	{
		buf[idx++] = state->input[state->i];
		state->i++;
		buf[idx] = '\0';
		return ft_strdup(buf);
	}
	while (state->input[state->i])
	{
		if (!state->in_single && !state->in_double && is_whitespace(state->input[state->i]))
			break;
		if (!state->in_single && !state->in_double && (state->input[state->i] == '(' || state->input[state->i] == ')'))
			break;
		process_token_char(state);
	}
	buf[idx] = '\0';
	return ft_strdup(buf);
}

void	process_token_char(t_token_state *state)
{
	char c;

	c = state->input[state->i];
	if (!state->in_single && !state->in_double && is_whitespace(c))
		return;
	if (c == '\'' && !state->in_double)
	{
		state->in_single = !state->in_single;
		state->i++;
		return;
	}
	if (c == '\"' && !state->in_single)
	{
		state->in_double = !state->in_double;
		state->i++;
		return;
	}
	if (c == '\\' && !state->in_single)
	{
		state->i++;
		if (!state->input[state->i])
			return;
		c = state->input[state->i];
	}
	if (c == '$' && !state->in_single)
	{
		if (state->input[state->i + 1] == '?')
			handle_dollar_question(state->input, &state->i, state->buffer, &state->buf_index);
		else
			handle_dollar_variable(state->input, &state->i, state->buffer, &state->buf_index);
		return;
	}
	state->buffer[state->buf_index++] = c;
	state->i++;
}

t_token	*lexer(const char *input)
{
	fprintf(stderr, "[debuger] Entree dans lexer() : \"%s\"\n", input);
	t_token *head;
	t_token *tail;
	t_token *token;
	size_t i;
	char *tok_str;

	head = NULL;
	tail = NULL;
	i = skip_whitespace(input, 0);
	while (input[i])
	{
		tok_str = collect_token(input);
		if (!tok_str)
			break;
		token = create_token(tok_str);
		free(tok_str);
		if (!token)
			break;
		if (!head)
			head = tail = token;
		else
		{
			tail->next = token;
			tail = token;
		}
		fprintf(stderr, "[debuger] Token cree : \"%s\"\n", token->value);
		i = skip_whitespace(input, i);
	}
	fprintf(stderr, "[debuger] Fin du lexer()\n");
	return (head);
}
