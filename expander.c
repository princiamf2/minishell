/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:52:07 by michel            #+#    #+#             */
/*   Updated: 2025/05/13 18:12:18 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	handle_param_expansion(t_token_state *state, t_buffer *buf,
		t_data *data)
{
	int		start;
	int		depth;
	char	*expr;
	char	*result;

	state->i += 2;
	start = state->i;
	depth = 1;
	while (state->input[state->i] && depth)
	{
		if (state->input[state->i] == '{')
			depth++;
		else if (state->input[state->i] == '}')
			depth--;
		state->i++;
	}
	expr = strndup(state->input + start, state->i - start - 1);
	result = expand_param(expr, data);
	free(expr);
	append_to_buffer(buf, result);
	free(result);
	return (true);
}

bool	handle_arith_expansion(t_token_state *st, t_buffer *buf, t_data *data)
{
	char	*expr;
	char	*expanded;
	long	value;
	char	*str_val;

	expr = extract_arith_expr(st);
	expanded = expand_vars_in_str(expr, data->env);
	free(expr);
	value = eval_arith(expanded);
	free(expanded);
	str_val = ft_itoa(value);
	append_to_buffer(buf, str_val);
	free(str_val);
	return (true);
}

bool	handle_dollar_subst(t_token_state *st, t_buffer *buf, t_data *data)
{
	char	*cmd;
	char	*out;

	cmd = extract_subst_cmd(st);
	if (!cmd || st->input[st->i - 1] != ')')
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		free(cmd);
		return (false);
	}
	out = run_command_substitution(cmd, data);
	free(cmd);
	if (!out)
		return (false);
	append_to_buffer(buf, out);
	free(out);
	return (true);
}

bool	handle_backtick_substitution(t_token_state *state, t_buffer *buf,
		t_data *data)
{
	int		start;
	char	*cmd;
	char	*out;

	state->i++;
	start = state->i;
	while (state->input[state->i] && state->input[state->i] != '`')
		state->i++;
	cmd = strndup(state->input + start, state->i - start);
	out = run_command_substitution(cmd, data);
	free(cmd);
	append_to_buffer(buf, out);
	free(out);
	state->i++;
	return (true);
}

char	*run_command_substitution(const char *cmd, t_data *data)
{
	char	*buf;
	char	*out;

	buf = read_subst_output(cmd, data);
	out = strdup(buf);
	free(buf);
	return (out);
}
