/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_extern.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:48:36 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:37:23 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_excutable(const char *cmd, t_env *env)
{
	char	*paths;
	char	*full_path;

	paths = get_path_env(env);
	if (!paths)
		return (NULL);
	full_path = search_executable_in_paths(paths, cmd);
	free(paths);
	return (full_path);
}

char	*get_executable_path(t_command *cmd, t_data *data)
{
	if (ft_strchr(cmd->args[0], '/') != NULL)
		return (ft_strdup(cmd->args[0]));
	return (find_excutable(cmd->args[0], data->env));
}

int	handle_exec_path_not_found(t_command *cmd, int saved_stdin,
		int saved_stdout)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd && cmd->args && cmd->args[0])
		ft_putstr_fd(cmd->args[0], 2);
	else
		ft_putstr_fd("(null)", 2);
	ft_putstr_fd(": command not found\n", 2);
	restore_original_fds(saved_stdin, saved_stdout);
	return (127);
}

int	execute_external_command(t_command *cmd, t_data *data)
{
	char	*exec_path;
	int		status;
	int		saved_stdin;
	int		saved_stdout;

	saved_stdin = -1;
	saved_stdout = -1;
	if (!save_original_fds(&saved_stdin, &saved_stdout))
		return (1);
	if (handle_redirection(cmd) < 0)
	{
		restore_original_fds(saved_stdin, saved_stdout);
		return (1);
	}
	exec_path = get_executable_path(cmd, data);
	if (!exec_path)
	{
		return (handle_exec_path_not_found(cmd, saved_stdin, saved_stdout));
	}
	status = fork_and_execute_no_redir(exec_path, cmd, data);
	free(exec_path);
	restore_original_fds(saved_stdin, saved_stdout);
	return (status);
}
