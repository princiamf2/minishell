/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   local_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:25:50 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:47:18 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_local_assignmment(char *input, t_data *data)
{
	char	*eq;

	eq = ft_strchr(input, '=');
	if (!eq)
		return ;
	*eq = '\0';
	local_set(&(data->local_vars), input, eq + 1);
	*eq = '=';
}

void	local_set(t_env **local, const char *key, const char *val)
{
	t_env	*node;

	node = find_env_node(*local, key);
	if (node)
		update_node_value(node, val);
	else
	{
		node = create_env_node2(key, val);
		if (!node)
		{
			ft_putendl_fd("local_set: malloc\n", 2);
			return ;
		}
		node->next = *local;
		*local = node;
	}
}
