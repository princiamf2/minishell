/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arith.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 20:38:18 by michel            #+#    #+#             */
/*   Updated: 2025/05/08 18:46:41 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long	parse_term(const char **s);
static long	parse_factor(const char **s);

static long	parse_expr(const char **s)
{
	long	res;
	long	rhs;
	char	op;

	res = parse_term(s);
	while (**s == '+' || **s == '-')
	{
		op = *(*s)++;
		rhs = parse_term(s);
		if (op == '+')
			res += rhs;
		else
			res -= rhs;
	}
	return (res);
}

static long	parse_factor(const char **s)
{
	long	res;

	while (**s == ' ')
		(*s)++;
	if (**s == '(')
	{
		(*s)++;
		res = parse_expr(s);
		if (**s == ')')
			(*s)++;
	}
	else
		res = strtol(*s, (char **)s, 10);
	return (res);
}

static long	parse_term(const char **s)
{
	long	res;
	long	rhs;

	res = parse_factor(s);
	while (**s == '*' || **s == '/')
	{
		if (**s == '*')
			res *= (rhs = parse_factor(&*s));
		else
			res /= (rhs = parse_factor(&*s));
	}
	return (res);
}

long	eval_arith(const char *s)
{
	const char	*ptr;

	ptr = s;
	return (parse_expr(&ptr));
}

char	*expand_vars_in_str(const char *s, t_env *env)
{
	int		i;
	char	*res;
	char	*name;
	char	*val;
	char	*tmp;
	int		start;

	i = 0;
	res = strdup("");
	while (s[i])
	{
		if (s[i] == '$' && (ft_isalpha(s[i + 1]) || s[i + 1] == '_'))
		{
			i++;
			start = i;
			while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
				i++;
			name = strndup(s + start, i - start);
			val = env_get(env, name);
			tmp = res;
			if (val)
				res = ft_strjoin(res, val);
			else
				res = ft_strjoin(res, "");
			free(tmp);
			free(name);
		}
		else
		{
			tmp = res;
			res = ft_strjoin(res, strndup(&s[i], 1));
			free(tmp);
			i++;
		}
	}
	return (res);
}
