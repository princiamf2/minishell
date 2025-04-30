/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing_matches.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:24:30 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:57:26 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ensure_globinfo_capacity(t_globinfo *info)
{
	char	**tmp;
	size_t	old_size;
	size_t	new_capacity;
	size_t	new_size_bytes;

	if (info->count >= info->capacity)
	{
		old_size = info->capacity * sizeof(char *);
		new_capacity = info->capacity * 2;
		if (new_capacity == 0)
			new_capacity = 8;
		new_size_bytes = new_capacity * sizeof(char *);
		tmp = ft_realloc(info->matches, old_size, new_size_bytes);
		if (!tmp)
		{
			perror("minishell: ft_realloc in add_match");
			return (false);
		}
		info->matches = tmp;
		info->capacity = new_capacity;
	}
	return (true);
}

int	add_match(t_globinfo *info, const char *dirpath, const char *filename)
{
	char	*fullpath;

	if (!ensure_globinfo_capacity(info))
	{
		return (-1);
	}
	fullpath = build_fullpath(dirpath, filename);
	if (!fullpath)
	{
		perror("minishell: build_fullpath in add_match");
		return (0);
	}
	info->matches[info->count] = fullpath;
	info->count++;
	info->matches[info->count] = NULL;
	return (1);
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

void	add_all_matches_to_cmd(char **matches, t_command *cmd, size_t *argc,
		size_t *capacity)
{
	size_t	i;

	i = 0;
	while (matches[i])
	{
		add_argument_to_cmd(cmd, matches[i], argc, capacity);
		i++;
	}
}

void	free_matches(char **matches)
{
	size_t	i;

	i = 0;
	while (matches[i])
	{
		free(matches[i]);
		i++;
	}
	free(matches);
}
