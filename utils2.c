/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:24:30 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/09 16:04:51 by mm-furi          ###   ########.fr       */
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

static bool	ensure_globinfo_capacity(t_globinfo *info)
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
