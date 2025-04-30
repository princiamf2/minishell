/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:08 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:50:19 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	try_expand_glob(t_token **cur, t_command *cmd, size_t *argc,
		size_t *capacity)
{
	char	**matches;

	matches = glob_pattern((*cur)->value);
	if (matches && matches[0])
	{
		add_all_matches_to_cmd(matches, cmd, argc, capacity);
		free_matches(matches);
		*cur = (*cur)->next;
		return (true);
	}
	else
	{
		if (matches)
			free_matches(matches);
		return (false);
	}
}

char	**glob_pattern(const char *pattern)
{
	char		dirpath[PATH_MAX];
	const char	*pat;

	if (!pattern)
		return (NULL);
	get_dir_and_pattern(pattern, dirpath, sizeof(dirpath), &pat);
	return (read_directory_matches(dirpath, pat));
}

void	get_dir_and_pattern(const char *pattern, char *dirpath, size_t size,
		const char **pat)
{
	const char	*slash;
	size_t		len;

	slash = strrchr(pattern, '/');
	if (slash)
	{
		len = slash - pattern;
		if (len < size)
		{
			ft_strncpy(dirpath, pattern, len);
			dirpath[len] = '\0';
		}
		else
		{
			ft_strncpy(dirpath, ".", size);
			dirpath[size - 1] = '\0';
		}
		*pat = slash + 1;
	}
	else
	{
		ft_strncpy(dirpath, ".", size);
		dirpath[size - 1] = '\0';
		*pat = pattern;
	}
}

char	**read_directory_matches(const char *dirpath, const char *pat)
{
	t_globinfo	info;

	info.dir = opendir(dirpath);
	if (!info.dir)
		return (NULL);
	info.capacity = 8;
	info.count = 0;
	info.matches = malloc(info.capacity * sizeof(char *));
	if (!info.matches)
	{
		closedir(info.dir);
		return (NULL);
	}
	if (process_directory_entries(&info, dirpath, pat) == -1)
	{
		closedir(info.dir);
		free(info.matches);
		return (NULL);
	}
	closedir(info.dir);
	return (finalize_matches(info.matches, info.count));
}

bool	match_pattern(const char *pattern, const char *str)
{
	size_t	skip;

	if (!*pattern)
		return (!*str);
	if (*pattern == '*')
	{
		skip = 0;
		while (true)
		{
			if (match_pattern(pattern + 1, str + skip))
				return (true);
			if (!str[skip])
				break ;
			skip++;
		}
		return (false);
	}
	return ((*pattern == *str) && match_pattern(pattern + 1, str + 1));
}
