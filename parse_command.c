/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:17:47 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:47:26 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parse_command(t_token **cur)
{
	if (!cur || !*cur)
		return (NULL);
	if ((*cur)->type == PAREN_OPEN)
		return (parse_subshell_command(cur));
	else
		return (parse_regular_command(cur));
}

t_command	*parse_regular_command(t_token **cur)
{
	t_command	*cmd;
	size_t		argc;
	size_t		capacity;

	if (!*cur || ((*cur)->type != WORD && !is_redirection(*cur)))
		return (NULL);
	cmd = init_regular_command_struct(&argc, &capacity);
	if (!cmd)
		return (NULL);
	if (!populate_and_finalize_command(cmd, cur, &argc, &capacity))
	{
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}

t_command	*init_regular_command_struct(size_t *argc_ptr, size_t *capacity_ptr)
{
	t_command	*cmd;

	*argc_ptr = 0;
	*capacity_ptr = 0;
	cmd = init_command();
	if (!cmd)
		return (NULL);
	if (init_cmd_args(cmd, capacity_ptr, argc_ptr) == -1)
	{
		free_command(cmd);
		return (NULL);
	}
	return (cmd);
}

bool	populate_and_finalize_command(t_command *cmd, t_token **cur,
		size_t *argc_ptr, size_t *capacity_ptr)
{
	collect_cmd_args(cmd, cur, argc_ptr, capacity_ptr);
	if (*argc_ptr == 0 && cmd->redirs == NULL && cmd->output == NULL
		&& cmd->input == NULL)
	{
		return (false);
	}
	if (cmd->args)
		cmd->args[*argc_ptr] = NULL;
	return (true);
}

t_command	*parse_subshell_command(t_token **cur)
{
	t_command	*cmd;
	size_t		argc;
	size_t		capacity;

	cmd = parse_subshell(cur);
	if (!cmd)
		return (NULL);
	if (init_cmd_args(cmd, &capacity, &argc) == -1)
	{
		free_command(cmd);
		return (NULL);
	}
	collect_cmd_args(cmd, cur, &argc, &capacity);
	if (cmd->args)
		cmd->args[argc] = NULL;
	return (cmd);
}
