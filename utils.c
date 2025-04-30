/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:24:42 by michel            #+#    #+#             */
/*   Updated: 2025/04/30 13:49:16 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

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

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (!ptr)
		return (malloc(new_size));
	if (new_size == 0)
	{
		free(ptr);
		return (NULL);
	}
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (old_size > 0)
	{
		if (new_size < old_size)
			old_size = new_size;
		ft_memcpy(new_ptr, ptr, old_size);
	}
	free(ptr);
	return (new_ptr);
}

char	*build_fullpath(const char *dirpath, const char *filename)
{
	size_t	len_dir;
	size_t	len_file;
	char	*fullpath;

	if (ft_strcmp(dirpath, ".") == 0)
		return (ft_strdup(filename));
	len_dir = ft_strlen(dirpath);
	len_file = ft_strlen(filename);
	fullpath = malloc(len_dir + 1 + len_file + 1);
	if (!fullpath)
		return (NULL);
	ft_strcpy(fullpath, dirpath);
	fullpath[len_dir] = '/';
	ft_strcpy(fullpath + len_dir + 1, filename);
	return (fullpath);
}
