/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:06:56 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/13 12:41:23 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
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

void	process_token_char(t_token_state *state, int exit_status, t_env *env)
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
			handle_dollar_question(state->input, &state->i, state->buffer, exit_status);
		else
			handle_dollar_variable(state->input, &state->i, state->buffer, env);
		return;
	}
	append_to_buffer(state->buffer, (char[]){c, '\0'});
	state->i++;
}

t_token *lexer(const char *input)
{
    t_token *head = NULL;
    t_token *tail = NULL;
    t_token *token;
    size_t i;
    t_token_state state;

    fprintf(stderr, "[debuger] Entree dans lexer() : \"%s\"\n", input);
    state.input = input;
    state.i = 0;
    state.in_single = false;
    state.in_double = false;

    // Allouer et initialiser le buffer
    state.buffer = malloc(sizeof(t_buffer));
    if (!state.buffer)
    {
        perror("malloc");
        return NULL;
    }
    state.buffer->cap = 4096;
    state.buffer->index = 0;
    state.buffer->str = malloc(state.buffer->cap);
    if (!state.buffer->str)
    {
        free(state.buffer);
        perror("malloc");
        return NULL;
    }

    i = skip_whitespace(input, 0);
    while (input[i])
    {
        // Réinitialiser l'index du buffer pour chaque token
        state.i = i;
        state.buffer->index = 0;
        token = create_token( collect_token(&state, 0, /*env*/ NULL) );
        if (!token)
            break;
        if (!head)
            head = tail = token;
        else
        {
            tail->next = token;
            tail = token;
        }
        i = skip_whitespace(input, state.i);
    }
    free(state.buffer->str);
    free(state.buffer);
    fprintf(stderr, "[debuger] Fin du lexer()\n");
    return head;
}
