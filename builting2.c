/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builting2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:36:34 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/08 20:28:52 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int builtin_unset(char **args, char ***env)
{
	int	i;
	int	index;
	int	j;

	i = 1;
	while (args[i])
	{
		index = find_env_index_unset(*env, args[i]);
		if (index >= 0)
		{
			free((*env)[index]);
			j = index;
			while ((*env)[j])
			{
				(*env)[j] = (*env)[j + 1];
				j++;
			}
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

