/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils7.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:46:08 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/27 14:35:16 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*read_heredoc(const char *delim, bool quoted)
{
	size_t	cap;
	size_t	len;
	char	*content;

	(void)quoted;
	cap = 1024;
	len = 0;
	content = malloc(cap);
	if (!content)
		exit(1);
	content[0] = '\0';
	while (!process_heredoc_line(delim, &content, &len, &cap))
		;
	return (content);
}

t_command	*parse_pipeline(t_token **cur)
{
	t_command	*first;
	t_command	*current;
	t_command	*next;

	first = parse_command(cur);
	if (!first)
		return (NULL);
	current = first;
	while (*cur && (*cur)->type == PIPE)
	{
		*cur = (*cur)->next;
		next = parse_command(cur);
		if (!next)
			break ;
		current->next_pipe = next;
		current = next;
	}
	return (first);
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

void	append_line(char **content, size_t *len, size_t *cap, const char *line)
{
	size_t	l_len;
	char	*tmp;
	size_t	old_size;

	l_len = ft_strlen(line);
	if (*len + l_len + 2 > *cap)
	{
		old_size = *cap;
		*cap += l_len + 1024;
		tmp = ft_realloc(*content, old_size, *cap);
		if (!tmp)
		{
			free(*content);
			exit(1);
		}
		*content = tmp;
	}
	ft_strcpy(*content + *len, line);
	*len += l_len;
	(*content)[(*len)++] = '\n';
	(*content)[*len] = '\0';
}
