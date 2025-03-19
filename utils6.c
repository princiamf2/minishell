/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:34:47 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/18 17:10:21 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <linux/limits.h>

bool skip_opening_paren(t_token **cur)
{
	if (!cur || !*cur || ft_strcmp((*cur)->value, "(") != 0)
		return false;
	*cur = (*cur)->next;
	return true;
}

t_token *collect_tokens_until_closing(t_token **cur)
{
	t_token *start;
	t_token *prev;
	int balance;

	start = *cur;
	prev = NULL;
	balance = 1;
	while (*cur)
	{
		if (ft_strcmp((*cur)->value, "(") == 0)
			balance++;
		else if (ft_strcmp((*cur)->value, ")") == 0)
		{
			balance--;
			if (balance == 0)
				break;
		}
		prev = *cur;
		*cur = (*cur)->next;
	}
	if (!*cur || balance != 0)
		return NULL;
	if (prev)
		prev->next = NULL;
	return start;
}

char **glob_pattern(const char *pattern)
{
	char dirpath[PATH_MAX];
	const char *pat;

	if (!pattern)
		return NULL;
	get_dir_and_pattern(pattern, dirpath, sizeof(dirpath), &pat);
	return read_directory_matches(dirpath, pat);
}
