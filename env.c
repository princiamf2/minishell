/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:05:24 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/18 15:32:45 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*env_get(t_env *env, const char *key)
{
	size_t key_len;

	key_len = ft_strlen(key);
	while (env)
	{
		if (ft_strncmp(env->key, key, key_len) == 0 && env->key[key_len] == '\0')
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	env_set(t_env **env, const char *key, const char *val)
{
	t_env *node;
	size_t key_len;

	node = *env;
	key_len = ft_strlen(key);
	while (node)
	{
		if (ft_strncmp(node->key, key, key_len) == 0 && node->key[key_len] == '\0')
		{
			free(node->value);
			node->value = ft_strdup(val);
			return ;
		}
		node = node->next;
	}
	node = malloc(sizeof(t_env));
	if (!node)
	{
		perror("malloc");
		return ;
	}
	node->key = ft_strdup(key);
	node->value = ft_strdup(val);
	node->next = *env;
	*env = node;
}

void	env_unset(t_env **env, const char *key)
{
	t_env *node;
	t_env *prev;
	size_t key_len;

	node = *env;
	prev = NULL;
	key_len = ft_strlen(key);
	while (node)
	{
		if (ft_strncmp(node->key, key, key_len) == 0 && node->key[key_len] == '\0')
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
		perror("malloc");
		return (NULL);
	}
	return (array);
}

void	convert_env_list_to_array(t_env *env, char **array)
{
	int		i;
	t_env	*node;
	char	*tmp;

	i = 0;
	node = env;
	while (node)
	{
		tmp = ft_strjoin(node->key, "=");
		array[i] = ft_strjoin(tmp, node->value);
		free(tmp);
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
