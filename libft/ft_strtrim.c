/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 17:11:01 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/02 16:14:33 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	i;

	i = 0;
	while (src[i] != '\0' && i < n)
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

static int	is_in_set(char c, const char *set)
{
	while (*set)
	{
		if (c == *set)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*start;
	char	*end;
	char	*str;

	if (!s1 || !set)
		return (NULL);
	start = (char *)s1;
	end = (char *)s1 + ft_strlen(s1);
	while (*start && is_in_set(*start, set))
		start++;
	while (end > start && is_in_set(*(end - 1), set))
		end--;
	str = (char *)malloc(end - start + 1);
	if (!str)
		return (NULL);
	ft_strncpy(str, start, end - start);
	str[end - start] = '\0';
	return (str);
}
