/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_dispatch.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 14:48:15 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:35:44 by nicolsan         ###   ########.fr       */
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

int	execute_builtin_with_redir(t_command *cmd, t_data *data)
{
	int	saved_stdin;
	int	saved_stdout;
	int	ret;

	saved_stdin = -1;
	saved_stdout = -1;
	if (!save_original_fds(&saved_stdin, &saved_stdout))
		return (1);
	if (handle_redirection(cmd) < 0)
	{
		restore_original_fds(saved_stdin, saved_stdout);
		return (1);
	}
	ret = execute_builtin(cmd->args, data);
	restore_original_fds(saved_stdin, saved_stdout);
	data->exit_status = ret;
	return (ret);
}
