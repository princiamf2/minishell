/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_subshell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:21:57 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:53:14 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*parse_subshell(t_token **cur)
{
	t_token		*sub_tokens_head;
	t_cmdlist	*sub_ast;
	t_command	*cmd;

	sub_tokens_head = collect_and_validate_subshell_tokens(cur);
	if (!sub_tokens_head)
	{
		return (NULL);
	}
	sub_ast = parse_subshell_tokens(sub_tokens_head);
	if (!sub_ast)
	{
		return (NULL);
	}
	cmd = create_subshell_command(sub_ast);
	if (!cmd)
	{
		free_cmdlist(sub_ast);
		return (NULL);
	}
	return (cmd);
}

t_token	*collect_and_validate_subshell_tokens(t_token **cur)
{
	t_token	*sub_tokens_head;

	if (!skip_opening_paren(cur))
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `('", 2);
		return (NULL);
	}
	sub_tokens_head = collect_tokens_until_closing(cur);
	if (!sub_tokens_head)
	{
		return (NULL);
	}
	return (sub_tokens_head);
}

t_cmdlist	*parse_subshell_tokens(t_token *sub_tokens_head)
{
	t_cmdlist	*sub_ast;

	sub_ast = parse_line(sub_tokens_head);
	free_tokens(sub_tokens_head);
	if (!sub_ast)
	{
		ft_putendl_fd("minishell: error parsing subshell content", 2);
		return (NULL);
	}
	return (sub_ast);
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
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->redirs = NULL;
	cmd->next_pipe = NULL;
	return (cmd);
}
