/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 15:50:12 by mm-furi           #+#    #+#             */
/*   Updated: 2024/10/02 19:24:27 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*save;

	if (!lst || !f || !del)
		return (0);
	new_lst = ft_lstnew(f(lst->content));
	if (!new_lst)
		return (0);
	save = new_lst;
	lst = lst->next;
	while (lst)
	{
		new_lst->next = ft_lstnew(f(lst->content));
		if (!new_lst->next)
		{
			ft_lstclear(&save, del);
			return (0);
		}
		new_lst = new_lst->next;
		lst = lst->next;
	}
	new_lst->next = NULL;
	return (save);
}
