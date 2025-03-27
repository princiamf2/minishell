/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils12.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:04:31 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/25 14:06:02 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_export_arguments(char **args, t_data *data)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (process_export_argument(args[i], data))
			return (1);
		i++;
	}
	return (0);
}

int	process_export_argument(char *arg, t_data *data)
{
	char	*eq;
	char	*value;
	char	*key;

	eq = ft_strchr(arg, '=');
	key = extract_key(arg, eq);
	value = extract_value(eq);
	if (!eq)
		return (0);
	if (!key || !value)
	{
		perror("ft_substr/ft_strdup");
		free(key);
		free(value);
		return (1);
	}
	if (update_env_var(data, key, value))
		free(key);
	else if (add_env_var(data, key, value))
		return (1);
	return (0);
}

char	*extract_key(char *arg, char *eq)
{
	size_t	key_len;

	key_len = eq - arg;
	return (ft_substr(arg, 0, key_len));
}

char	*extract_value(char *eq)
{
	return (ft_strdup(eq + 1));
}

void	free_env_variable(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}
