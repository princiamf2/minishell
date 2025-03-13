/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builting2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:36:34 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/13 15:40:18 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_unset(char **args, t_data *data)
{
	int	i;
	t_env *node;
	t_env *prev;

	i = 1;
	while (args[i])
	{
		prev = NULL;
		node = data->env;
		while (node)
		{
			if (ft_strcmp(node->key, args[i]) == 0)
			{
				if (prev)
					prev->next = node->next;
				else
					data->env = node->next;
				free(node->key);
				free(node->value);
				free(node);
				break;
			}
			prev = node;
			node = node->next;
		}
		i++;
	}
	return (0);
}

int builtin_env(char **env)
{
	int i;

	i = 0;
	while (env[i])
	{
		ft_putstr_fd(env[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
	return (0);
}

int builtin_cd(char **args)
{
	char *path;

	if (!args[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putstr_fd("cd : HOME not set\n", 2);
			return (1);
		}
	}
	else
		path = args[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

