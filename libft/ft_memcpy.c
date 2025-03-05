/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 12:07:34 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/03 17:07:25 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*dest1;
	unsigned char	*src1;

	if (n == 0)
		return (dest);
	if (dest == NULL || src == NULL)
		return (NULL);
	dest1 = (unsigned char *)dest;
	src1 = (unsigned char *)src;
	while (n--)
		*dest1++ = *src1++;
	return (dest);
}
