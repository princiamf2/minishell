/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expader_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 17:42:14 by mm-furi           #+#    #+#             */
/*   Updated: 2025/05/13 17:53:16 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_char(char *res, char c)
{
	char	*tmp;

	tmp = ft_strjoin(res, strndup(&c, 1));
	free(res);
	return (tmp);
}

char	*append_var(char *res, const char *s, t_env *env, int *i)
{
	int		start;
	char	*name;
	char	*val;
	char	*to_join;
	char	*tmp;

	(*i)++;
	start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	name = strndup(s + start, *i - start);
	val = env_get(env, name);
	if (val)
		to_join = val;
	else
		to_join = "";
	tmp = ft_strjoin(res, to_join);
	free(res);
	free(name);
	return (tmp);
}
