/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:07:51 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/07 13:50:15 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	ensure_args_capacity(t_command *cmd, size_t argc, size_t *capacity)
{
	char	**tmp;
	size_t	old_size;
	size_t	new_capacity;

	if (argc + 1 >= *capacity)
	{
		old_size = *capacity * sizeof(char *);
		new_capacity = *capacity * 2;
		if (new_capacity == 0)
			new_capacity = 8;
		tmp = ft_realloc(cmd->args, old_size, new_capacity * sizeof(char *));
		if (!tmp)
		{
			perror("minishell: ft_realloc in add_argument_to_cmd");
			return (false);
		}
		cmd->args = tmp;
		*capacity = new_capacity;
	}
	return (true);
}

void	add_argument_to_cmd(t_command *cmd, const char *arg, size_t *argc,
			size_t *capacity)
{
	if (!cmd || !arg || !argc || !capacity || !cmd->args)
		return ;
	if (!ensure_args_capacity(cmd, *argc, capacity))
	{
		return ;
	}
	cmd->args[*argc] = ft_strdup(arg);
	if (!cmd->args[*argc])
	{
		perror("minishell: ft_strdup in add_argument_to_cmd");
		cmd->args[*argc] = NULL;
		return ;
	}
	(*argc)++;
	cmd->args[*argc] = NULL;
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
