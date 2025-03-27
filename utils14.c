/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils14.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:39:57 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/27 14:38:25 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	process_heredoc_line(const char *delim, char **content, size_t *len,
		size_t *cap)
{
	char	*line;

	line = readline("> ");
	if (!line)
		return (true);
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (true);
	}
	append_line(content, len, cap, line);
	free(line);
	return (false);
}

bool	is_valid_entry(struct dirent *entry)
{
	return (ft_strcmp(entry->d_name, ".") != 0 && ft_strcmp(entry->d_name,
			"..") != 0);
}

int	process_directory_entries(t_globinfo *info, const char *dirpath,
		const char *pat)
{
	struct dirent	*entry;

	entry = readdir(info->dir);
	while (entry != NULL)
	{
		if (!is_valid_entry(entry))
			continue ;
		if (match_pattern(pat, entry->d_name))
		{
			if (add_match(info, dirpath, entry->d_name) == -1)
				return (-1);
		}
		entry = readdir(info->dir);
	}
	return (0);
}

char	**finalize_matches(char **matches, size_t count)
{
	char	**tmp;
	size_t	old_size;

	old_size = (count + 1) * sizeof(char *);
	tmp = ft_realloc(matches, old_size, (count + 1) * sizeof(char *));
	if (tmp)
	{
		tmp[count] = NULL;
		return (tmp);
	}
	return (matches);
}

void	free_pipeline(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next_pipe;
		free_command(cmd);
		cmd = next;
	}
}
