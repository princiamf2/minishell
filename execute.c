/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:48:36 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/02 16:09:47 by mm-furi          ###   ########.fr       */
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

int	execute_external_command(t_command *cmd, t_data *data)
{
	char	*exec_path;
	int		status;

	exec_path = get_executable_path(cmd, data);
	if (!exec_path)
		return (127);
	status = fork_and_execute(exec_path, cmd, data);
	free(exec_path);
	return (status);
}

int	execute_command(t_command *cmd, t_data *data)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;
	int	no_args_ret;

	saved_stdin = save_stdin();
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin < 0)
	{
		perror("dup");
		return (1);
	}
	no_args_ret = handle_no_command_arguments(cmd, saved_stdin, saved_stdout);
	if (no_args_ret != -1)
		return (no_args_ret);
	if (is_builtins(cmd->args[0]))
		return (execute_builtin_with_redir(cmd, data));
	status = execute_external_command(cmd, data);
	restore_stdin(saved_stdin);
	if (dup2(saved_stdout, STDOUT_FILENO) < 0)
		perror("dup2 stdout");
	close(saved_stdout);
	data->exit_status = status;
	return (status);
}

int	execute_full_command(t_command *cmd, t_data *data)
{
	if (cmd->next_pipe)
		return (execute_pipeline(cmd, data));
	else
		return (execute_command(cmd, data));
}
