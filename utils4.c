/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:03:00 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/26 19:34:23 by mm-furi          ###   ########.fr       */
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

	capacity = 8;
	argc = 0;
	cmd->args = malloc(capacity * sizeof(char *));
	if (!cmd->args)
	{
		free(cmd);
		return (-1);
	}
	while (*cur && (((*cur)->type == WORD) || is_redirection(*cur)))
	{
		if ((*cur)->type == WORD)
			handle_word_token(cur, cmd, &argc, &capacity);
		else
			parse_redirection(cmd, cur);
	}
	cmd->args[argc] = NULL;
	return ((int)argc);
}

void	handle_word_token(t_token **cur, t_command *cmd, size_t *argc,
		size_t *capacity)
{
	char	**matches;
	size_t	mi;

	if (strchr((*cur)->value, '*') && !(*cur)->quoted)
	{
		matches = glob_pattern((*cur)->value);
		if (matches && matches[0])
		{
			mi = 0;
			while (matches[mi])
			{
				add_argument_to_cmd(cmd, matches[mi], argc, capacity);
				mi++;
			}
			mi = 0;
			while (matches[mi])
			{
				free(matches[mi]);
				mi++;
			}
			free(matches);
			*cur = (*cur)->next;
			return ;
		}
		else
		{
			if (matches)
				free(matches);
		}
	}
	add_argument_to_cmd(cmd, (*cur)->value, argc, capacity);
	*cur = (*cur)->next;
}
