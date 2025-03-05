/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 14:34:54 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/02 19:39:04 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t n)
{
	size_t	i;
	size_t	j;

	i = 0;
	if (!*to_find)
		return ((char *)str);
	while (str[i] && i < n)
	{
		j = 0;
		while (str[i + j] && to_find[j]
			&& str[i + j] == to_find[j] && i + j < n)
			j++;
		if (!to_find[j])
			return ((char *)str + i);
		i++;
	}
	return (0);
}
