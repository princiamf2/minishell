/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 15:57:51 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/03 16:57:35 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t i, size_t c)
{
	void	*str;

	str = malloc(i * c);
	if (str == NULL)
		return (NULL);
	ft_bzero(str, i * c);
	return (str);
}
