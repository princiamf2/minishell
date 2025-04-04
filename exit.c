/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 14:25:50 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/04 17:43:25 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !str[0])
		return (0);
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	free_all_data(t_data *data)
{
	if (!data)
		return ;
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	if (data->tokens)
	{
		free_tokens(data->tokens);
		data->tokens = NULL;
	}
	if (data->cmdlist)
	{
		free_cmdlist(data->cmdlist);
		data->cmdlist = NULL;
	}
	if (data->env)
	{
		free_env(data->env);
		data->env = NULL;
	}
	free(data);
}

int	builtin_exit(char **args, t_data *data)
{
	int	exit_status;

	exit_status = data->exit_status;
	if (args[1])
	{
		if (!is_numeric(args[1]))
		{
			ft_putstr_fd("minishell: exit: numeric argument required\n", 2);
			exit_status = 255;
			return (exit_status);
		}
		else if (args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			return (1);
		}
		else
			exit_status = ft_atoi(args[1]) % 256;
	}
	else
		exit_status = data->exit_status;
	ft_putendl_fd("exit", 2);
	free_all_data(data);
	exit(exit_status);
	return (exit_status);
}

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
