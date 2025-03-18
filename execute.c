/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:48:36 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/18 15:24:51 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *get_path_env(void)
{
	char *path_env;

	path_env = getenv("PATH");

	if (!path_env)
		return (NULL);

	return (ft_strdup(path_env));
}

char	*build_executable_path(const char *directory, const char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(directory, "/");
	full_path = ft_strjoin(temp, cmd);
	free(temp);

	return (full_path);
}

char	*search_executable_in_paths(char *paths, const char *cmd)
{
	char	*token;
	char	*full_path;

	token = ft_strtok(paths, ":");

	while (token)
	{
		full_path = build_executable_path(token, cmd);
		if (!full_path)
			return (NULL);

		if (access(full_path, X_OK) == 0)
			return (full_path);

		free(full_path);
		token = ft_strtok(NULL, ":");
	}
	return (NULL);
}

char	*find_excutable(const char *cmd)
{
	char	*paths;
	char	*full_path;

	paths = get_path_env();
	if (!paths)
		return (NULL);
	full_path = search_executable_in_paths(paths, cmd);

	free(paths);

	return (full_path);
}

int	save_and_restore_stdin(int save)
{
	if (save)
		return (dup(STDIN_FILENO));
	else
	{
		dup2(save, STDIN_FILENO);
		close(save);
	}
	return (0);
}

char	*get_executable_path(t_command *cmd)
{
	if (ft_strchr(cmd->args[0], '/') != NULL)
		return (ft_strdup(cmd->args[0]));
	return (find_excutable(cmd->args[0]));
}

int	fork_and_execute(char *exec_path, t_command *cmd, t_data *data)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(exec_path);
		return (1);
	}
	if (pid == 0)
	{
		ft_putstr_fd("pid = 0\n", 1);
		if (execve(exec_path, cmd->args, env_to_array(data->env)) == -1)
		{
			perror("execve");
			exit(1);
		}
	}
	waitpid(pid, &status, 0);
	return (WIFEXITED(status)) ? WEXITSTATUS(status) : 1;
}

int	execute_external_command(t_command *cmd, t_data *data, int saved_stdin)
{
	char	*exec_path;
	int		status;

	exec_path = get_executable_path(cmd);
	if (!exec_path)
	{
		ft_putstr_fd("exec_path pas trouvé\n", 2);
		save_and_restore_stdin(saved_stdin);
		return (127);
	}

	status = fork_and_execute(exec_path, cmd, data);
	free(exec_path);
	save_and_restore_stdin(saved_stdin);
	return (status);
}

int	execute_command(t_command *cmd, t_data *data)
{
	int	saved_stdin;
	int	status;

	saved_stdin = save_and_restore_stdin(1);
	if (saved_stdin < 0)
	{
		perror("dup");
		return (1);
	}

	if (is_builtins(cmd->args[0]))
		return (execute_builtin_with_redir(cmd, data));

	if (handle_redirection(cmd) < 0)
		return (1);

	status = execute_external_command(cmd, data, saved_stdin);
	return (status);
}

int execute_full_command(t_command *cmd, t_data *data)
{
	ft_putstr_fd("rentre dans full_command\n", 1);
	if (cmd->next_pipe)
		return execute_pipeline(cmd, data);
	else
		return execute_command(cmd, data);
}
