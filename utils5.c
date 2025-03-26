/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:07:51 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/26 19:34:28 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_argument_to_cmd(t_command *cmd, const char *arg, size_t *argc,
		size_t *capacity)
{
	char	**tmp;

	if (*argc + 1 >= *capacity)
	{
		*capacity *= 2;
		tmp = realloc(cmd->args, *capacity * sizeof(char *));
		if (!tmp)
		{
			perror("realloc");
			return ;
		}
		cmd->args = tmp;
	}
	cmd->args[(*argc)++] = ft_strdup(arg);
}

void	parse_redirection(t_command *cmd, t_token **cur)
{
	if ((*cur)->type == REDIR_OUT || (*cur)->type == REDIR_APPEND)
	{
		cmd->output = ft_strdup((*cur)->next->value);
		cmd->append = ((*cur)->type == REDIR_APPEND);
		*cur = (*cur)->next->next;
	}
	else if ((*cur)->type == REDIR_IN)
	{
		cmd->input = ft_strdup((*cur)->next->value);
		*cur = (*cur)->next->next;
	}
	else if ((*cur)->type == HEREDOC)
	{
		cmd->heredoc = 1;
		cmd->input = ft_strdup((*cur)->next->value);
		*cur = (*cur)->next->next;
	}
}

t_andor	*parse_andor(t_token **cur)
{
	t_command		*pipeline;
	t_andor			*head;
	t_token_type	op;
	t_command		*next_pipeline;
	t_andor			*node;

	pipeline = parse_pipeline(cur);
	if (!pipeline)
		return (NULL);
	head = init_andor(pipeline);
	while (*cur && ((*cur)->type == AND_IF || (*cur)->type == OR_IF))
	{
		op = (*cur)->type;
		*cur = (*cur)->next;
		next_pipeline = parse_pipeline(cur);
		if (!next_pipeline)
			break ;
		node = init_andor(next_pipeline);
		if (!node)
			break ;
		node->op = op;
		append_andor(head, node);
	}
	return (head);
}

void	append_andor(t_andor *head, t_andor *node)
{
	t_andor	*cur;

	cur = head;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

t_andor	*init_andor(t_command *pipeline)
{
	t_andor	*andor;

	andor = malloc(sizeof(t_andor));
	if (!andor)
	{
		perror("malloc");
		return (NULL);
	}
	andor->pipeline = pipeline;
	andor->op = 0;
	andor->next = NULL;
	return (andor);
}
