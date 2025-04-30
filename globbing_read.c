/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globbing_read.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:39:57 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:47:53 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		{
			entry = readdir(info->dir);
			continue ;
		}
		if (match_pattern(pat, entry->d_name))
		{
			if (add_match(info, dirpath, entry->d_name) == -1)
				return (-1);
		}
		entry = readdir(info->dir);
	}
	return (0);
}
