/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:51:00 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 15:07:07 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_empty_raw_token(char *raw_value, t_token_state *state,
		size_t *i_ptr)
{
	if (ft_strlen(raw_value) == 0)
	{
		free(raw_value);
		*i_ptr = skip_whitespace(state->input, state->i);
		return (true);
	}
	return (false);
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

void	qualify_token_quoting(t_token *token, t_token_state *state,
		size_t start_idx)
{
	size_t	end_idx;
	bool	is_single_quoted;
	bool	is_double_quoted;

	end_idx = state->i;
	if (end_idx > start_idx && (end_idx - start_idx >= 2))
	{
		is_single_quoted = (state->input[start_idx] == '\''
				&& state->input[end_idx - 1] == '\'');
		is_double_quoted = (state->input[start_idx] == '\"'
				&& state->input[end_idx - 1] == '\"');
		if (is_single_quoted || is_double_quoted)
		{
			token->quoted = true;
		}
	}
}

void	print_tokens_debug(t_token *tokens)
{
	printf("--- Tokens ---\n");
	while (tokens)
	{
		printf("Type: %d, Value: [%s]\n", tokens->type, tokens->value);
		tokens = tokens->next;
	}
	printf("--------------\n");
}

void	append_char_as_string(t_buffer *buffer, char c)
{
	char	char_str[2];

	char_str[0] = c;
	char_str[1] = '\0';
	append_to_buffer(buffer, char_str);
}
