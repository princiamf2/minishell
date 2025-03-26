/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:39:45 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/26 19:33:31 by mm-furi          ###   ########.fr       */
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
