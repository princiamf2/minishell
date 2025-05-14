/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:07:36 by mm-furi           #+#    #+#             */
/*   Updated: 2025/05/13 18:15:03 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*extract_arith_expr(t_token_state *st)
{
	int	start;
	int	depth;

	start = st->i + 3;
	depth = 2;
	while (st->input[st->i] && depth)
	{
		if (st->input[st->i] == '(')
			depth++;
		else if (st->input[st->i] == ')')
			depth--;
		st->i++;
	}
	return (strndup(st->input + start, st->i - start - 1));
}

char	*extract_subst_cmd(t_token_state *st)
{
	int	start;
	int	depth;

	st->i += 2;
	start = st->i;
	depth = 1;
	while (st->input[st->i] && depth)
	{
		if (st->input[st->i] == '(')
			depth++;
		else if (st->input[st->i] == ')')
			depth--;
		st->i++;
	}
	return (strndup(st->input + start, st->i - start - 1));
}

char	*read_subst_output(const char *cmd, t_data *data)
{
	int		fd[2];
	pid_t	pid;
	char	*buf;
	int		r;

	pipe(fd);
	pid = fork();
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		exec_line(cmd, data);
		exit(EXIT_SUCCESS);
	}
	close(fd[1]);
	buf = malloc(4096);
	r = read(fd[0], buf, 4095);
	buf[r] = '\0';
	waitpid(pid, NULL, 0);
	close(fd[0]);
	if (r > 0 && buf[r - 1] == '\n')
		buf[r - 1] = '\0';
	return (buf);
}

char	*get_env_value(const char *var, t_data *data)
{
	char	*val;

	val = env_get(data->local_vars, var);
	if (!val)
		val = env_get(data->env, var);
	return (val);
}

char	*apply_param_op(const char *var, const char *op, const char *word,
		t_data *data)
{
	char	*val;
	char	*res;

	val = get_env_value(var, data);
	if ((!val || !*val) && strcmp(op, ":-") == 0)
		res = strdup(word);
	else if ((!val || !*val) && strcmp(op, ":=") == 0)
	{
		env_set(&data->env, var, word);
		res = strdup(word);
	}
	else if (val && *val)
		res = strdup(val);
	else
		res = strdup("");
	return (res);
}
