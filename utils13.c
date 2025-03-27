/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils13.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:33:07 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/25 17:01:11 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_all_matches_to_cmd(char **matches, t_command *cmd, size_t *argc,
		size_t *capacity)
{
	size_t	i;

	i = 0;
	while (matches[i])
	{
		add_argument_to_cmd(cmd, matches[i], argc, capacity);
		i++;
	}
}

void	free_matches(char **matches)
{
	size_t	i;

	i = 0;
	while (matches[i])
	{
		free(matches[i]);
		i++;
	}
	free(matches);
}

bool	try_expand_glob(t_token **cur, t_command *cmd, size_t *argc,
		size_t *capacity)
{
	char	**matches;

	matches = glob_pattern((*cur)->value);
	if (matches && matches[0])
	{
		add_all_matches_to_cmd(matches, cmd, argc, capacity);
		free_matches(matches);
		*cur = (*cur)->next;
		return (true);
	}
	else
	{
		if (matches)
			free_matches(matches);
		return (false);
	}
}

void	free_command_args(t_command *cmd)
{
	int	i;

	if (!cmd->args)
		return ;
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
