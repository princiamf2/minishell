/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 14:50:21 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/02 19:25:15 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*new_content;

	new_content = (t_list *)malloc(sizeof(t_list) * 1);
	if (!new_content)
		return (NULL);
	new_content -> content = content;
	new_content -> next = NULL;
	return (new_content);
}
