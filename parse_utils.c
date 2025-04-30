/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:03:00 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:49:32 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmdlist	*build_subshell_ast(t_token *tokens)
{
	return (parse_line(tokens));
}

int	parse_command_arguments(t_command *cmd, t_token **cur)
{
	size_t	capacity;
	size_t	argc;

	if (init_cmd_args(cmd, &capacity, &argc) == -1)
		return (-1);
	collect_cmd_args(cmd, cur, &argc, &capacity);
	if (argc == 0)
	{
		free(cmd->args);
		cmd->args = NULL;
		return (0);
	}
	cmd->args[argc] = NULL;
	return ((int)argc);
}
