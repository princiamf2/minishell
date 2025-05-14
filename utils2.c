/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/14 20:07:13 by mm-furi           #+#    #+#             */
/*   Updated: 2025/05/14 20:17:52 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_one_heredoc(t_command *cmd)
{
	int	fd;

	if (!cmd->heredoc)
		return (0);
	fd = open_tmp_heredoc_file(&cmd->heredoc_tmp);
	if (fd < 0)
		return (perror("open heredoc"), -1);
	if (read_and_write_heredoc(fd, cmd->input) < 0)
	{
		close(fd);
		unlink(cmd->heredoc_tmp);
		free(cmd->heredoc_tmp);
		return (-1);
	}
	close(fd);
	return (0);
}

int	process_pipeline_heredocs(t_command *cmd)
{
	int	ret;

	while (cmd)
	{
		ret = process_one_heredoc(cmd);
		if (ret < 0)
			return (-1);
		cmd = cmd->next_pipe;
	}
	return (0);
}

void	increment_shlvl(char ***envp)
{
	int		i;
	int		level;
	char	*old;
	char	*new_val;
	char	*joined;

	i = 0;
	while ((*envp)[i] && ft_strncmp((*envp)[i], "SHLVL=", 6) != 0)
		i++;
	if ((*envp)[i])
	{
		old = (*envp)[i] + 6;
		level = ft_atoi(old) + 1;
		new_val = ft_itoa(level);
		joined = ft_strjoin("SHLVL=", new_val);
		free(new_val);
		free((*envp)[i]);
		(*envp)[i] = joined;
	}
	else
		*envp = append_env(*envp, "SHLVL=1");
}

char	**copy_env(char **envp)
{
	char	**copy;
	size_t	count;
	size_t	i;

	count = 0;
	while (envp[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		if (!copy[i])
		{
			while (i--)
				free(copy[i]);
			free(copy);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	copy[count] = NULL;
	return (copy);
}

char	**append_env(char **envp, char *newvar)
{
	char	**new_env;
	size_t	count;
	size_t	i;

	count = 0;
	while (envp[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < count)
	{
		new_env[i] = envp[i];
		i++;
	}
	new_env[i++] = ft_strdup(newvar);
	if (!new_env[i - 1])
		exit(EXIT_FAILURE);
	new_env[i] = NULL;
	free(envp);
	return (new_env);
}
