/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_path_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:12:20 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:41:14 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path_env(t_env *env)
{
	char	*path_env;

	path_env = env_get(env, "PATH");
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
