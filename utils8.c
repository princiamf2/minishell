/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils8.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:24:30 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/27 14:40:48 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

bool	is_redirection(t_token *token)
{
	return (token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == REDIR_APPEND || token->type == HEREDOC);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

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
	return (dest);
}

int	add_match(t_globinfo *info, const char *dirpath, const char *filename)
{
	char	*fullpath;
	char	**tmp;
	size_t	old_size;

	if (info->count >= info->capacity)
	{
		old_size = info->capacity * sizeof(char *);
		info->capacity *= 2;
		tmp = ft_realloc(info->matches, old_size, info->capacity
				* sizeof(char *));
		if (!tmp)
			return (-1);
		info->matches = tmp;
	}
	fullpath = build_fullpath(dirpath, filename);
	if (!fullpath)
		return (0);
	info->matches[info->count++] = fullpath;
	return (1);
}
