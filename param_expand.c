/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   param_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:00:22 by michel            #+#    #+#             */
/*   Updated: 2025/05/13 18:16:48 by mm-furi          ###   ########.fr       */
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
	char	*res;

	var = extract_varname(expr);
	op = extract_op(expr + strlen(var));
	word = extract_word(expr + strlen(var) + strlen(op), op);
	res = apply_param_op(var, op, word, data);
	free(var);
	free(op);
	free(word);
	return (res);
}
