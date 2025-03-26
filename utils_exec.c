/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:48:50 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/26 19:33:36 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path_env(void)
{
	char	*path_env;

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

int	save_stdin(void)
{
	int	fd;

	fd = dup(STDIN_FILENO);
	if (fd < 0)
		perror("dup");
	return (fd);
}

void	restore_stdin(int saved)
{
	if (dup2(saved, STDIN_FILENO) < 0)
		perror("dup2");
	close(saved);
}
