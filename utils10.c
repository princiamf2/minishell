/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils10.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:14:29 by michel            #+#    #+#             */
/*   Updated: 2025/03/13 12:13:05 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cmdlist(t_cmdlist *commands, t_env *env)
{
	int			last_status;
	t_cmdlist	*node;

	last_status = 0;
	node = commands;
	while (node)
	{
		last_status = execute_andor(node->andor, env);
		node = node->next;
	}
	return (last_status);
}

int execute_andor(t_andor *list, t_env *env)
{
	int result = 0;
	bool should_run = true;
	t_andor *node = list;
	while (node)
	{
		if (should_run)
			result = execute_full_command(node->pipeline, env, env_to_array(env));
		if (node->op == AND_IF)
			should_run = (result == 0);
		else if (node->op == OR_IF)
			should_run = (result != 0);
		else
			should_run = true;
		node = node->next;
	}
	return result;
}

void free_cmdlist(t_cmdlist *list)
{
    t_cmdlist *next;
    while (list)
    {
        next = list->next;
        free_andor(list->andor);
        free(list);
        list = next;
    }
}

void free_andor(t_andor *andor)
{
    t_andor *next;
    while (andor)
    {
        next = andor->next;
        free_command(andor->pipeline);
        free(andor);
        andor = next;
    }
}

