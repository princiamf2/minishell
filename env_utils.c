/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:07:18 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:37:17 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_env_variables(t_env *env)
{
	t_env	*node;
	int		count;

	count = 0;
	node = env;
	while (node)
	{
		count++;
		node = node->next;
	}
	return (count);
}

char	**allocate_env_array(int count)
{
	char	**array;

	array = malloc((count + 1) * sizeof(char *));
	if (!array)
	{
		perror("minishell: malloc (allocate_env_array)");
		return (NULL);
	}
	return (array);
}

char	*format_env_node_to_string(t_env *node)
{
	char	*entry_str;
	size_t	key_len;
	size_t	val_len;
	size_t	total_len;

	key_len = ft_strlen(node->key);
	val_len = ft_strlen(node->value);
	total_len = key_len + 1 + val_len;
	entry_str = malloc(total_len + 1);
	if (!entry_str)
	{
		perror("minishell: malloc (format_env_node_to_string)");
		return (NULL);
	}
	ft_memcpy(entry_str, node->key, key_len);
	entry_str[key_len] = '=';
	ft_memcpy(entry_str + key_len + 1, node->value, val_len);
	entry_str[total_len] = '\0';
	return (entry_str);
}

void	convert_env_list_to_array(t_env *env, char **array)
{
	int		i;
	t_env	*node;

	i = 0;
	node = env;
	while (node)
	{
		if (!node->key || !node->value)
		{
			node = node->next;
			continue ;
		}
		array[i] = format_env_node_to_string(node);
		if (!array[i])
		{
			return ;
		}
		i++;
		node = node->next;
	}
	array[i] = NULL;
}

char	**env_to_array(t_env *env)
{
	int		count;
	char	**array;

	count = count_env_variables(env);
	array = allocate_env_array(count);
	if (!array)
		return (NULL);
	convert_env_list_to_array(env, array);
	return (array);
}
