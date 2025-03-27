/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:50:12 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/25 14:52:34 by mm-furi          ###   ########.fr       */
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
