/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:41:19 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/03 17:00:24 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *str, int c, size_t n)
{
	size_t			i;
	unsigned char	*str1;

	i = 0;
	str1 = (unsigned char *)str;
	while (i < n)
	{
		if (str1[i] == (unsigned char)c)
			return (&str1[i]);
		i++;
	}
	return (NULL);
}
