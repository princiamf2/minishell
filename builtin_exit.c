/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:45:57 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:58:44 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	calculate_exit_code(char **args, int current_status)
{
	int	status;

	if (!args[1])
	{
		status = current_status;
	}
	else
	{
		if (!is_numeric(args[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			status = 255;
		}
		else
		{
			status = ft_atoi(args[1]) % 256;
		}
	}
	return (status);
}

int	builtin_exit(char **args, t_data *data)
{
	int	final_exit_status;

	ft_putendl_fd("exit", 2);
	if (args[1] && args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		data->exit_status = 1;
		return (1);
	}
	final_exit_status = calculate_exit_code(args, data->exit_status);
	free_all_data(data);
	exit(final_exit_status);
	return (final_exit_status);
}
