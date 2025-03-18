/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:48:15 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/18 15:12:57 by mm-furi          ###   ########.fr       */
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

int execute_builtin(char **args, t_data *data)
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

/*int builtin_export(char **args, t_data *data)
{
	int		i;
	char	*eq;
	char	*key;
	char	*value;
	size_t	key_len;

	if (!args[1])
		return (builtin_env(env_to_array(data->env)));
	i = 1;
	while (args[i])
	{
		eq = ft_strchr(args[i], '=');
		if (!eq)
		{
			i++;
			continue ;
		}
		key_len = eq - args[i];
		key = ft_substr(args[i], 0, key_len);
		value = ft_strdup(eq + 1);
		if (!key || !value)
		{
			perror("ft_substr/ft_strdup");
			free(key);
			free(value);
			return (1);
		}
		if (update_env_var(data, key, value))
			free(key);
		else if (add_env_var(data, key, value))
			return (1);
		i++;
	}
	return (0);
}
*/

//fonctions pour separer builtin_export

int	builtin_export(char **args, t_data *data)
{
	if (!args[1])
		return (builtin_env(env_to_array(data->env)));

	return (handle_export_arguments(args, data));
}

int	handle_export_arguments(char **args, t_data *data)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (process_export_argument(args[i], data))
			return (1);
		i++;
	}
	return (0);
}

int	process_export_argument(char *arg, t_data *data)
{
	char	*eq;
	char	*value;
	char	*key;

	eq = ft_strchr(arg, '=');
	key = extract_key(arg, eq);
	value = extract_value(eq);
	if (!eq)
		return (0);
	if (!key || !value)
	{
		perror("ft_substr/ft_strdup");
		free(key);
		free(value);
		return (1);
	}
	if (update_env_var(data, key, value))
		free(key);
	else if (add_env_var(data, key, value))
		return (1);

	return (0);
}

char	*extract_key(char *arg, char *eq)
{
	size_t	key_len;

	key_len = eq - arg;
	return (ft_substr(arg, 0, key_len));
}

char	*extract_value(char *eq)
{
	return (ft_strdup(eq + 1));
}
// fin de la decoupe
