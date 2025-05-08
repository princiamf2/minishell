/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_expand_var.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:33:04 by nicolsan          #+#    #+#             */
/*   Updated: 2025/05/08 18:39:17 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_dollar_sign(t_token_state *state, int exit_status_UNUSED,
		t_data *data)
{
	(void)exit_status_UNUSED;
	if (state->input[state->i] != '$' || state->in_single)
		return ;
	if (state->input[state->i + 1] == '?')
	{
		handle_dollar_question(state->input, &state->i, state->buffer, data);
		return ;
	}
	if (state->input[state->i + 1] == '{')
	{
		handle_param_expansion(state, state->buffer, data);
		return ;
	}
	if (state->input[state->i + 1] == '(' && state->input[state->i + 2] == '(')
	{
		handle_arith_expansion(state, state->buffer, data);
		return ;
	}
	if (state->input[state->i + 1] == '(')
	{
		handle_dollar_subst(state, state->buffer, data);
		return ;
	}
	handle_dollar_variable(state->input, &state->i, state->buffer, data);
}

void	handle_dollar_question(const char *input, size_t *i, t_buffer *buf,
		t_data *data)
{
	char	*num_str;

	(void)input;
	if (!data)
	{
		num_str = ft_strdup("0");
	}
	else
	{
		num_str = ft_itoa(data->exit_status);
	}
	if (!num_str)
	{
		perror("minishell: ft_itoa failed for exit status");
		num_str = ft_strdup("?");
		if (!num_str)
			return ;
	}
	append_to_buffer(buf, num_str);
	free(num_str);
	*i += 2;
}

void	handle_dollar_variable(const char *input, size_t *i, t_buffer *buf,
		t_data *data)
{
	size_t	start;
	size_t	var_len;
	char	var_name[256];
	char	*value;

	start = *i + 1;
	var_len = extract_variable_name(input, start, var_name, sizeof(var_name));
	if (var_len > 0)
	{
		value = find_variable_value(var_name, data);
		append_to_buffer(buf, value);
		*i = start + var_len;
	}
	else
	{
		append_to_buffer(buf, "$");
		(*i)++;
	}
}

size_t	extract_variable_name(const char *input, size_t start, char *var_buffer,
		size_t buffer_size)
{
	size_t	len;

	len = 0;
	while (input[start + len] && (ft_isalnum(input[start + len]) || input[start
				+ len] == '_'))
	{
		len++;
	}
	if (len > 0)
	{
		if (len >= buffer_size)
			len = buffer_size - 1;
		ft_strncpy(var_buffer, input + start, len);
		var_buffer[len] = '\0';
	}
	else
	{
		if (buffer_size > 0)
			var_buffer[0] = '\0';
	}
	return (len);
}

char	*find_variable_value(const char *var_name, t_data *data)
{
	char	*val;

	val = env_get(data->env, var_name);
	if (!val)
	{
		val = env_get(data->local_vars, var_name);
		if (!val)
			val = "";
	}
	return (val);
}
