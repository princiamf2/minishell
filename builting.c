/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:48:15 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/08 20:34:31 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int is_builtins(char *cmd)
{
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}

int execute_builtin(char **args, char **envp)
{
	if (!ft_strcmp(args[0], "cd"))
		return (builtin_cd(args));
	if (!ft_strcmp(args[0], "echo"))
		return (builtin_echo(args));
	if (!ft_strcmp(args[0], "pwd"))
		return (builtin_pwd(args));
	if (!ft_strcmp(args[0], "export"))
		return (builtin_export(args, &envp));
	if (!ft_strcmp(args[0], "unset"))
		return (builtin_unset(args, &envp));
	if (!ft_strcmp(args[0], "env"))
		return (builtin_env(envp));
	if (!ft_strcmp(args[0], "exit"))
		exit(0);
	return (0);
}

int builtin_echo(char **args)
{
	int i;
	int nline;

	i = 1;
	nline = 1;
	if (args[i] && ft_strcmp(args[i], "-n") == 0)
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

int builtin_pwd(char **args)
{
	char path[4096];

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

int builtin_export(char **args, char ***env)
{
	int		i;
	int		index;
	char	*eq;

	if (!args[1])
		return (builtin_env(*env));
	i = 1;
	while (args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (!eq)
		{
			i++;
			continue;
		}
		index = find_env_index(*env, args[i]);
		if (index >= 0)
		{
			free((*env)[index]);
			(*env)[index] = ft_strdup(args[i]);
		}
		else
		{
			*env = add_env_variable(*env, args[i]);
			if (!*env)
				return (1);
		}
		i++;
	}
	return (0);
}
