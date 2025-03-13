/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils8.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:24:30 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/12 20:56:08 by michel           ###   ########.fr       */
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

bool is_redirection(t_token *token)
{
	return (token->type == REDIR_IN || token->type == REDIR_OUT ||
			token->type == REDIR_APPEND || token->type == HEREDOC);
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

char *ft_strncpy(char *dest, const char *src, size_t n)
{
    size_t i;

    i = 0;
    while (i < n && src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }
    return dest;
}

int add_match(char ***matches, size_t *count, size_t *capacity, const char *dirpath, const char *filename)
{
	if (*count >= *capacity)
	{
		*capacity *= 2;
		char **tmp = realloc(*matches, (*capacity) * sizeof(char *));
		if (!tmp)
			return -1;
		*matches = tmp;
	}
	char *fullpath = build_fullpath(dirpath, filename);
	if (!fullpath)
		return 0;
	(*matches)[(*count)++] = fullpath;
	return 1;
}
