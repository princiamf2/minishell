/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:03:00 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/01 17:16:30 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmdlist	*build_subshell_ast(t_token *tokens)
{
	return (parse_line(tokens));
}

t_token	*extract_subshell_tokens(t_token **cur)
{
	if (!skip_opening_paren(cur))
		return (NULL);
	return (collect_tokens_until_closing(cur));
}

t_command	*create_subshell_command(t_cmdlist *sub_ast)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc");
		return (NULL);
	}
	cmd->subshell = true;
	cmd->subshell_ast = sub_ast;
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next_pipe = NULL;
	return (cmd);
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

void	handle_word_token(t_token **cur, t_command *cmd, size_t *argc,
		size_t *capacity)
{
	if (strchr((*cur)->value, '*') && !(*cur)->quoted)
	{
		if (try_expand_glob(cur, cmd, argc, capacity))
			return ;
	}
	add_argument_to_cmd(cmd, (*cur)->value, argc, capacity);
	*cur = (*cur)->next;
}
