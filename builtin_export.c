/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:32:26 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:58:55 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_export(char **args, t_data *data)
{
	if (!args || !args[0])
		return (1);
	if (!args[1])
	{
		return (handle_export_no_args(data));
	}
	else
	{
		return (handle_export_arguments(args, data));
	}
}

int	handle_export_no_args(t_data *data)
{
	char	**env_array;
	int		i;
	int		count;

	env_array = env_to_array(data->env);
	if (!env_array)
		return (1);
	count = 0;
	while (env_array[count] != NULL)
		count++;
	if (count > 0)
		qsort(env_array, count, sizeof(char *), compare_env_vars);
	i = 0;
	while (env_array[i])
	{
		print_export_formatted(env_array[i]);
		i++;
	}
	free_env_array(env_array);
	return (0);
}

void	print_export_formatted(const char *env_var)
{
	const char	*equal_sign;

	if (!env_var)
		return ;
	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign)
	{
		print_export_key_value(env_var, equal_sign);
	}
	else
	{
		print_export_key_only(env_var);
	}
}

void	print_export_key_value(const char *env_var, const char *equal_sign)
{
	char		*key;
	const char	*value;
	size_t		key_len;

	key_len = equal_sign - env_var;
	key = malloc(key_len + 1);
	if (key)
	{
		ft_memcpy(key, env_var, key_len);
		key[key_len] = '\0';
		value = equal_sign + 1;
		ft_putstr_fd("declare -x ", 1);
		ft_putstr_fd(key, 1);
		ft_putstr_fd("=\"", 1);
		ft_putstr_fd((char *)value, 1);
		ft_putstr_fd("\"\n", 1);
		free(key);
	}
	else
	{
		perror("minishell: malloc (print_export_key_value key)");
		ft_putstr_fd((char *)env_var, 1);
		ft_putchar_fd('\n', 1);
	}
}

/* Helper pour afficher une var sans valeur assignee (declare -x KEY). */
void	print_export_key_only(const char *env_var)
{
	ft_putstr_fd("declare -x ", 1);
	ft_putstr_fd((char *)env_var, 1);
	ft_putchar_fd('\n', 1);
}
