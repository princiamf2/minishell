/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 14:47:37 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/07 12:55:15 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmdlist	*create_cmdlist_node(t_token **tokens)
{
	t_cmdlist	*node;

	node = malloc(sizeof(t_cmdlist));
	if (!node)
	{
		perror("malloc");
		return (NULL);
	}
	node->andor = parse_andor(tokens);
	node->next = NULL;
	return (node);
}

void	append_node(t_cmdlist **head, t_cmdlist **tail, t_cmdlist *node)
{
	if (*head == NULL)
	{
		*head = node;
		*tail = node;
	}
	else
	{
		(*tail)->next = node;
		*tail = node;
	}
}

t_cmdlist	*parse_line(t_token *tokens)
{
	t_cmdlist	*head;
	t_cmdlist	*tail;
	t_cmdlist	*node;

	if (!tokens)
		return (NULL);
	head = NULL;
	tail = NULL;
	while (tokens)
	{
		node = create_cmdlist_node(&tokens);
		if (!node)
			break ;
		append_node(&head, &tail, node);
		skip_semicolon(&tokens);
	}
	return (head);
}

t_command	*parse_regular_command(t_token **cur)
{
	t_command	*cmd;
	size_t		argc;
	size_t		capacity;

	cmd = init_command();
	if (!cmd)
		return (NULL);
	if (init_cmd_args(cmd, &capacity, &argc) == -1)
	{
		free_command(cmd);
		return (NULL);
	}
	collect_cmd_args(cmd, cur, &argc, &capacity);
	if (argc == 0 && cmd->redirs == NULL && !cmd->output && !cmd->input
		&& !cmd->heredoc)
	{
		free_command(cmd);
		return (NULL);
	}
	cmd->args[argc] = NULL;
	return (cmd);
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

t_command	*parse_command(t_token **cur)
{
	if (!cur || !*cur)
		return (NULL);
	if ((*cur)->type == PAREN_OPEN)
	{
		return (parse_subshell_command(cur));
	}
	else
	{
		return (parse_regular_command(cur));
	}
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
