/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:28:04 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:36:06 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_echo(char **args)
{
	int	i;
	int	nline;

	i = 1;
	nline = 1;
	while (args[i] && is_option_n(args[i]))
	{
		nline = 0;
		i++;
	}
	while (args[i])
	{
		ft_putstr_fd(args[i], 1);
		if (args[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (nline)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	builtin_pwd(char **args)
{
	char	path[4096];

	(void)args;
	if (getcwd(path, sizeof(path)) == NULL)
	{
		perror("pwd");
		return (1);
	}
	ft_putstr_fd(path, 1);
	ft_putchar_fd('\n', 1);
	return (0);
}

int	builtin_cd(char **args)
{
	char	*path;

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

int	builtin_env(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		ft_putstr_fd(env[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
	return (0);
}
