/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:34:08 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/25 13:19:16 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strtok(char *s, const char *delim)
{
	static char	*static_str;
	char		*token;

	if (s)
		static_str = s;
	if (!static_str || !delim)
		return (NULL);
	while (*static_str && ft_strchr(delim, *static_str))
		static_str++;
	if (!*static_str)
	{
		static_str = NULL;
		return (NULL);
	}
	token = static_str;
	while (*static_str && !ft_strchr(delim, *static_str))
		static_str++;
	if (*static_str)
	{
		*static_str = '\0';
		static_str++;
	}
	return (token);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}

int	find_env_index(char **env, char *var)
{
	int		i;
	size_t	key_len;
	char	*eq;

	i = 0;
	eq = ft_strchr(var, '=');
	if (!eq)
		return (-1);
	key_len = eq - var;
	while (env[i])
	{
		if (!ft_strncmp(env[i], var, key_len) && env[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	**add_env_variable(char **env, char *new_var)
{
	int		cnt;
	int		i;
	char	**new_env;

	cnt = 0;
	while (env[cnt])
		cnt++;
	new_env = malloc((cnt + 2) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < cnt)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;
	free(env);
	return (new_env);
}

int	find_env_index_unset(char **env, char *var)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (!ft_strncmp(env[i], var, len) && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}
