/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 13:29:40 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/01 13:34:14 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *str, int c)
{
	int		i;
	char	ch;

	i = 0;
	ch = (char)c;
	if (ch == '\0')
	{
		i = ft_strlen(str);
		return ((char *)&str[i]);
	}
	while (str[i])
	{
		if (str[i] == ch)
			return ((char *)&str[i]);
		i++;
	}
	return (NULL);
}
