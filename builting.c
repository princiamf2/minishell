/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:48:15 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/09 18:18:27 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtins(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") || !ft_strcmp(cmd, "echo") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"));
}

int	execute_builtin(char **args, t_data *data)
{
	if (!ft_strcmp(args[0], "cd"))
		return (builtin_cd(args));
	if (!ft_strcmp(args[0], "echo"))
		return (builtin_echo(args));
	if (!ft_strcmp(args[0], "pwd"))
		return (builtin_pwd(args));
	if (!ft_strcmp(args[0], "export"))
		return (builtin_export(args, data));
	if (!ft_strcmp(args[0], "unset"))
		return (builtin_unset(args, data));
	if (!ft_strcmp(args[0], "env"))
		return (builtin_env(env_to_array(data->env)));
	if (!ft_strcmp(args[0], "exit"))
		return (builtin_exit(args, data));
	return (0);
}

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

int	builtin_export(char **args, t_data *data)
{
	if (!args[1])
		return (builtin_env(env_to_array(data->env)));
	return (handle_export_arguments(args, data));
}
