/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_add_remove.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 19:51:48 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:59:33 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_unset(t_env **env, const char *key)
{
	t_env	*node;
	t_env	*prev;
	size_t	key_len;

	node = *env;
	prev = NULL;
	key_len = ft_strlen(key);
	while (node)
	{
		if (ft_strncmp(node->key, key, key_len) == 0
			&& node->key[key_len] == '\0')
		{
			if (prev)
				prev->next = node->next;
			else
				*env = node->next;
			free(node->key);
			free(node->value);
			free(node);
			return ;
		}
		prev = node;
		node = node->next;
	}
}

t_env	*create_env_node2(const char *key, const char *val)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
	{
		perror("malloc");
		return (NULL);
	}
	node->key = ft_strdup(key);
	node->value = ft_strdup(val);
	node->next = NULL;
	return (node);
}

int	add_env_var(t_data *data, char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("malloc");
		free(key);
		free(value);
		return (1);
	}
	new_node->key = key;
	new_node->value = value;
	new_node->next = data->env;
	data->env = new_node;
	return (0);
}

void	free_env_variable(t_env *node)
{
	if (!node)
		return ;
	free(node->key);
	free(node->value);
	free(node);
}
