/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 17:36:34 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:55:56 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(char **args, t_data *data)
{
	if (!args[1])
		return (0);
	return (handle_unset_arguments(args, data));
}

int	handle_unset_arguments(char **args, t_data *data)
{
	int	i;
	int	ret_status;

	ret_status = 0;
	i = 1;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			ret_status = 1;
		}
		else
		{
			unset_env_variable(&(data->env), args[i]);
		}
		i++;
	}
	return (ret_status);
}

void	unset_env_variable(t_env **env, char *key)
{
	t_env	*node;
	t_env	*prev;

	node = *env;
	prev = NULL;
	while (node)
	{
		if (ft_strcmp(node->key, key) == 0)
		{
			if (prev)
				prev->next = node->next;
			else
				*env = node->next;
			free_env_variable(node);
			return ;
		}
		prev = node;
		node = node->next;
	}
}
