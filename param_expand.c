/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:00:22 by michel            #+#    #+#             */
/*   Updated: 2025/05/08 18:34:12 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*extract_varname(const char *expr)
{
	int		len;
	char	*var;

	len = 0;
	while (expr[len] && expr[len] != ':' && expr[len] != '\0')
		len++;
	var = strndup(expr, len);
	return (var);
}

static char	*extract_op(const char *expr)
{
	if (expr[0] == ':' && expr[1])
		return (strndup(expr, 2));
	return (strndup(expr, 1));
}

static char	*extract_word(const char *expr, const char *op)
{
	int	ofs;

	ofs = strlen(op);
	return (strdup(expr + ofs));
}

char	*expand_param(const char *expr, t_data *data)
{
	char	*var;
	char	*op;
	char	*word;
	char	*val;
	char	*res;

	var = extract_varname(expr);
	op = extract_op(expr + strlen(var));
	word = extract_word(expr + strlen(var) + strlen(op), op);
	val = env_get(data->local_vars, var);
	if (!val)
		val = env_get(data->env, var);
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
	free(var);
	free(op);
	free(word);
	return (res);
}
