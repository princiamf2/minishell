/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bultin_export_utils1.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 18:07:35 by mm-furi           #+#    #+#             */
/*   Updated: 2025/05/15 14:50:57 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_export_no_assign(t_data *data, t_export_kv *kv)
{
	char	*current;
	char	*exists_in_env;

	current = env_get(data->local_vars, kv->key);
	if (current)
	{
		env_set(&data->env, kv->key, current);
		env_unset(&data->local_vars, kv->key);
	}
	else
	{
		exists_in_env = env_get(data->env, kv->key);
		if (!exists_in_env)
			env_set(&data->env, kv->key, "");
	}
}

int	do_export_append(t_data *data, t_export_kv *kv)
{
	char	*current;
	char	*suffix;
	char	*to_set;
	int		ret;

	current = env_get(data->env, kv->key);
	suffix = interpret_raw_value(kv->raw_value);
	to_set = current ? ft_strjoin(current, suffix) : ft_strjoin("", suffix);
	free(suffix);
	free(kv->raw_value);
	kv->raw_value = NULL;
	ret = update_or_add_env_var(data, kv->key, to_set);
	return (ret);
}

int	do_export_assign(t_data *data, t_export_kv *kv)
{
	char	*to_set;
	int		ret;

	to_set = interpret_raw_value(kv->raw_value);
	free(kv->raw_value);
	kv->raw_value = NULL;
	ret = update_or_add_env_var(data, kv->key, to_set);
	return (ret);
}
