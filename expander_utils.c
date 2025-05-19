/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:06:46 by mm-furi           #+#    #+#             */
/*   Updated: 2025/05/15 14:51:34 by mm-furi          ###   ########.fr       */
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

void	handle_append_flag(char *arg, t_export_kv *kv)
{
	char	*plus;

	kv->append = false;
	plus = ft_strnstr(arg, "+=", ft_strlen(arg));
	if (plus)
	{
		kv->append = true;
		kv->eq = plus;
		kv->key = extract_key(arg, plus);
		kv->raw_value = ft_strdup(plus + 2);
	}
}

void	handle_equal_sign(char *arg, t_export_kv *kv)
{
	if (kv->eq == NULL)
		kv->eq = ft_strchr(arg, '=');
	if (kv->eq)
	{
		kv->key = extract_key(arg, kv->eq);
		kv->raw_value = extract_value(kv->eq);
	}
	else
	{
		kv->eq = NULL;
		kv->key = ft_strdup(arg);
		kv->raw_value = NULL;
	}
}
