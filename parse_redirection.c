/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:40:53 by nicolsan          #+#    #+#             */
/*   Updated: 2025/05/14 20:08:25 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parse_redirection(t_command *cmd, t_token **cur)
{
	t_token	*redir_token;
	t_token	*target_token;
	bool	success;

	redir_token = *cur;
	if (!redir_token->next || !redir_token->next->value)
		return ;
	target_token = redir_token->next;
	success = false;
	if (redir_token->type == REDIR_OUT || redir_token->type == REDIR_APPEND)
		success = handle_redir_output(cmd, target_token->value,
				redir_token->type);
	else if (redir_token->type == REDIR_IN)
		success = handle_redir_input(cmd, target_token->value);
	else if (redir_token->type == HEREDOC)
		success = handle_redir_heredoc(cmd, target_token->value);
	if (success)
		*cur = target_token->next;
}

bool	handle_redir_output(t_command *cmd, const char *target_val,
		t_token_type type)
{
	if (cmd->output)
		free(cmd->output);
	cmd->output = interpret_raw_value(target_val);
	if (!cmd->output)
	{
		perror("minishell: interpret_raw_value for output failed");
		return (false);
	}
	cmd->append = (type == REDIR_APPEND);
	return (true);
}

bool	handle_redir_input(t_command *cmd, const char *target_val)
{
	if (cmd->input)
		free(cmd->input);
	cmd->input = interpret_raw_value(target_val);
	if (!cmd->input)
	{
		perror("minishell: interpret_raw_value for input failed");
		return (false);
	}
	cmd->heredoc = 0;
	return (true);
}

bool	handle_redir_heredoc(t_command *cmd, const char *target_val)
{
	if (cmd->input)
		free(cmd->input);
	cmd->input = ft_strdup(target_val);
	if (!cmd->input)
	{
		perror("minishell: ft_strdup for heredoc delimiter failed");
		return (false);
	}
	cmd->heredoc = 1;
	return (true);
}

int	prepare_all_heredocs(t_cmdlist *cmdlist)
{
	t_andor	*node;
	int		ret;

	node = cmdlist->andor;
	while (node)
	{
		ret = process_pipeline_heredocs(node->pipeline);
		if (ret < 0)
			return (-1);
		node = node->next;
	}
	return (0);
}
