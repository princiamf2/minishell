/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:04:31 by mm-furi           #+#    #+#             */
/*   Updated: 2025/05/08 18:42:42 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	extract_export_kv(char *arg, t_export_kv *kv)
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
	if (!kv->key)
	{
		perror("minishell: strdup/substr failed for export key");
		free(kv->raw_value);
		return (false);
	}
	return (true);
}

bool	validate_export_key(const char *key, char *arg_for_error)
{
	if (!is_valid_identifier(key))
	{
		ft_putstr_fd("minishell: export: `", 2);
		ft_putstr_fd(arg_for_error, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (false);
	}
	return (true);
}

int	update_or_add_env_var(t_data *data, char *key, char *final_value)
{
	if (update_env_var(data, key, final_value))
	{
		free(key);
		free(final_value);
	}
	else
	{
		if (add_env_var(data, key, final_value))
		{
			return (1);
		}
	}
	return (0);
}

int	process_and_update_env(t_data *data, t_export_kv *kv)
{
	if (!kv->eq && !kv->append)
	{
		do_export_no_assign(data, kv);
		free(kv->key);
		return (0);
	}
	if (kv->append)
		return (do_export_append(data, kv));
	return (do_export_assign(data, kv));
}
