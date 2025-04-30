/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:04:31 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:31:28 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	extract_export_kv(char *arg, t_export_kv *kv)
{
	kv->eq = ft_strchr(arg, '=');
	if (kv->eq)
	{
		kv->key = extract_key(arg, kv->eq);
		kv->raw_value = extract_value(kv->eq);
	}
	else
	{
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

/* Orchestre la mise a jour de l'env pour export (interpretation valeur,
	ajout/modif). */
int	process_and_update_env(t_data *data, t_export_kv *kv)
{
	char	*final_value;

	if (!kv->eq || !kv->raw_value)
	{
		free(kv->key);
		free(kv->raw_value);
		return (0);
	}
	final_value = interpret_raw_value(kv->raw_value);
	free(kv->raw_value);
	kv->raw_value = NULL;
	if (!final_value)
	{
		perror("minishell: interpret_raw_value failed for export value");
		free(kv->key);
		return (1);
	}
	return (update_or_add_env_var(data, kv->key, final_value));
}
