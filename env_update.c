/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:57:07 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:37:02 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_env_node(t_env *head, const char *key)
{
	size_t	key_len;

	key_len = ft_strlen(key);
	while (head)
	{
		if (ft_strncmp(head->key, key, key_len) == 0
			&& head->key[key_len] == '\0')
			return (head);
		head = head->next;
	}
	return (NULL);
}

void	update_node_value(t_env *node, const char *val)
{
	free(node->value);
	node->value = ft_strdup(val);
}

int	update_env_var(t_data *data, char *key, char *value)
{
	t_env	*node;
	size_t	key_len;

	node = data->env;
	key_len = ft_strlen(key);
	while (node)
	{
		if (ft_strncmp(node->key, key, key_len) == 0
			&& node->key[key_len] == '\0')
		{
			free(node->value);
			node->value = ft_strdup(value);
			return (1);
		}
		node = node->next;
	}
	return (0);
}
