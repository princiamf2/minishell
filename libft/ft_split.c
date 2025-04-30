/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 14:42:32 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/04 14:42:39 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		write(1, &str[i], 1);
		i++;
	}
}

int	is_delim(char c, char delim)
{
	if (c == delim)
		return (1);
	return (0);
}

int	count_words(const char *str, char delim)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		while (str[i] && is_delim(str[i], delim))
			i++;
		if (str[i] && !is_delim(str[i], delim))
		{
			j++;
			while (str[i] && !is_delim(str[i], delim))
				i++;
		}
	}
	return (j);
}

char	*ft_strdup2(const char *str, int start, int end)
{
	char	*dup;
	int		i;

	i = 0;
	dup = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!dup)
		return (NULL);
	while (start < end)
	{
		dup[i] = str[start];
		start++;
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	int		j;
	int		k;
	char	**result;

	result = (char **)malloc(sizeof(char *) * (count_words(s, c) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] && is_delim(s[i], c))
			i++;
		k = i;
		while (s[i] && !is_delim(s[i], c))
			i++;
		if (k < i)
			result[j++] = ft_strdup2(s, k, i);
	}
	result[j] = NULL;
	return (result);
}
