/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils8.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:24:30 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/11 18:05:03 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool match_pattern(const char *pattern, const char *str)
{
	size_t skip;

	if (!*pattern)
		return !*str;
	if (*pattern == '*')
	{
		skip = 0;
		while (true)
		{
			if (match_pattern(pattern + 1, str + skip))
				return true;
			if (!str[skip])
				break;
			skip++;
		}
		return false;
	}
	return (*pattern == *str) && match_pattern(pattern + 1, str + 1);
}

void free_env(t_env *env)
{
    t_env *tmp;
    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}
