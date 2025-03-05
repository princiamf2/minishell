/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:34:08 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/05 19:24:13 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

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

int ft_strcmp(char *s1, char *s2)
{
	int i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (s1[i] - s2[i]);
		i++;
	}
	return (s1[i] - s2[i]);
}
