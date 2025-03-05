/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 16:04:46 by mm-furi           #+#    #+#             */
/*   Updated: 2024/11/18 16:33:38 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*dup;
	size_t	len;

	if (!s1)
	{
		ft_putstr_fd("Erreur : chaîne NULL dans ft_strdup\n", 2);
		return (NULL);
	}
	len = ft_strlen(s1);
	dup = malloc(len + 1);
	if (!dup)
	{
		ft_putstr_fd("Erreur d'allocation mémoire dans ft_strdup\n", 2);
		return (NULL);
	}
	ft_strlcpy(dup, s1, len + 1);
	return (dup);
}
