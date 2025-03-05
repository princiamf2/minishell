/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:15:10 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/02 19:28:47 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*dest1;
	unsigned char	*src1;
	size_t			i;

	if (n == 0)
		return (dest);
	if (dest == NULL || src == NULL)
		return (NULL);
	dest1 = (unsigned char *)dest;
	src1 = (unsigned char *)src;
	i = 0;
	if (dest1 > src1)
	{
		while (n--)
			dest1[n] = src1[n];
	}
	else
	{
		while (i < n)
		{
			dest1[i] = src1[i];
			i++;
		}
	}
	return (dest1);
}
