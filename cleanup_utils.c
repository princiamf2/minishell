/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:33:07 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:59:13 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command_args(t_command *cmd)
{
	int	i;

	if (!cmd || !cmd->args)
	{
		return ;
	}
	i = 0;
	while (cmd->args[i])
	{
		free(cmd->args[i]);
		i++;
	}
	free(cmd->args);
	cmd->args = NULL;
}

void	free_command_redirs(t_command *cmd)
{
	t_redir	*redir;
	t_redir	*tmp;

	redir = cmd->redirs;
	while (redir)
	{
		tmp = redir->next;
		if (redir->target)
			free(redir->target);
		free(redir);
		redir = tmp;
	}
	cmd->redirs = NULL;
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	free_pipeline(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next_pipe;
		free_command(cmd);
		cmd = next;
	}
}

void	free_str_array(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}
