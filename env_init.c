/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:05:24 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:36:53 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*env_init(char **envp)
{
	t_env	*head;
	t_env	*node;
	int		i;

	head = NULL;
	i = 0;
	while (envp[i])
	{
		node = create_env_node(envp[i]);
		add_env_node(&head, node);
		i++;
	}
	return (head);
}

t_env	*create_env_node(char *env_entry)
{
	t_env	*node;
	char	*equal;

	equal = ft_strchr(env_entry, '=');
	if (!equal)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
	{
		perror("malloc");
		return (NULL);
	}
	node->key = ft_substr(env_entry, 0, equal - env_entry);
	node->value = ft_strdup(equal + 1);
	node->next = NULL;
	return (node);
}

void	add_env_node(t_env **head, t_env *new_node)
{
	if (!new_node)
		return ;
	new_node->next = *head;
	*head = new_node;
}

char	*env_get(t_env *env, const char *key)
{
	size_t	key_len;

	key_len = ft_strlen(key);
	while (env)
	{
		if (ft_strncmp(env->key, key, key_len) == 0
			&& env->key[key_len] == '\0')
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	env_set(t_env **env, const char *key, const char *val)
{
	t_env	*node;

	node = find_env_node(*env, key);
	if (node)
	{
		update_node_value(node, val);
		return ;
	}
	node = create_env_node2(key, val);
	if (!node)
		return ;
	node->next = *env;
	*env = node;
}
