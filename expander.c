/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:52:07 by michel            #+#    #+#             */
/*   Updated: 2025/05/08 18:35:37 by mm-furi          ###   ########.fr       */
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

bool	handle_arith_expansion(t_token_state *state, t_buffer *buf,
		t_data *data)
{
	int		start;
	int		depth;
	char	*expr;
	char	*tmp;
	long	value;
	char	*str_val;

	state->i += 3;
	start = state->i;
	depth = 2;
	while (state->input[state->i] && depth)
	{
		if (state->input[state->i] == '(')
			depth++;
		else if (state->input[state->i] == ')')
			depth--;
		state->i++;
	}
	expr = strndup(state->input + start, state->i - start - 1);
	tmp = expand_vars_in_str(expr, data->env);
	free(expr);
	value = eval_arith(tmp);
	free(tmp);
	str_val = ft_itoa(value);
	append_to_buffer(buf, str_val);
	free(str_val);
	return (true);
}

bool	handle_dollar_subst(t_token_state *state, t_buffer *buf, t_data *data)
{
	int		start;
	int		depth;
	char	*cmd;
	char	*out;

	state->i += 2;
	start = state->i;
	depth = 1;
	while (state->input[state->i] && depth)
	{
		if (state->input[state->i] == '(')
			depth++;
		else if (state->input[state->i] == ')')
			depth--;
		state->i++;
	}
	if (depth != 0)
	{
		ft_putstr_fd("minishell: syntax error\n", 2);
		return (false);
	}
	cmd = strndup(state->input + start, state->i - start - 1);
	if (!cmd)
		return (false);
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
	int		fd[2];
	pid_t	pid;
	char	*buf;
	char	*out;
	int		r;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		exec_line(cmd, data);
		exit(0);
	}
	close(fd[1]);
	buf = malloc(4096);
	r = read(fd[0], buf, 4095);
	buf[r] = '\0';
	waitpid(pid, NULL, 0);
	close(fd[0]);
	if (r > 0 && buf[r - 1] == '\n')
		buf[r - 1] = '\0';
	out = strdup(buf);
	free(buf);
	return (out);
}
