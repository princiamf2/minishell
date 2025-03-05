/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:36:35 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/02 19:39:45 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	get_actual_length(char const *s, unsigned int start, size_t len)
{
	size_t	s_len;
	size_t	actual_len;

	s_len = ft_strlen(s);
	if (start >= s_len)
	{
		actual_len = 0;
	}
	else
	{
		if (s_len - start < len)
			actual_len = s_len - start;
		else
			actual_len = len;
	}
	return (actual_len);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	actual_len;
	size_t	j;
	char	*substr;

	if (!s)
		return (NULL);
	actual_len = get_actual_length(s, start, len);
	substr = (char *)malloc(sizeof(char) * (actual_len + 1));
	if (!substr)
		return (NULL);
	j = 0;
	while (j < actual_len && s[start + j])
	{
		substr[j] = s[start + j];
		j++;
	}
	substr[j] = '\0';
	return (substr);
}
