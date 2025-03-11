/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:07:51 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/11 16:54:13 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void add_argument_to_cmd(t_command *cmd, const char *arg, size_t *argc, size_t *capacity)
{
	char **tmp;

	if (*argc + 1 >= *capacity)
	{
		*capacity *= 2;
		tmp = realloc(cmd->args, *capacity * sizeof(char *));
		if (!tmp)
		{
			perror("realloc");
			return;
		}
		cmd->args = tmp;
	}
	cmd->args[(*argc)++] = ft_strdup(arg);
}

void parse_redirection(t_command *cmd, t_token **cur)
{
	t_redir *redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
	{ perror("malloc"); return; }
	redir->type = (*cur)->type;
	redir->next = cmd->redirs;
	cmd->redirs = redir;
	*cur = (*cur)->next;
	if (!*cur || (*cur)->type != WORD)
	{
		fprintf(stderr, "[debuger] Erreur: redirection sans cible\n");
		free(redir);
		return;
	}
	if (redir->type == HEREDOC)
		redir->target = read_heredoc((*cur)->value, (*cur)->quoted);
	else
		redir->target = ft_strdup((*cur)->value);
	redir->delim_quoted = (*cur)->quoted;
	*cur = (*cur)->next;
}

t_andor *parse_andor(t_token **cur)
{
	t_command *pipeline;
	t_andor *head;
	t_token_type op;
	t_command *next_pipeline;
	t_andor *node;

	fprintf(stderr, "[debuger] Entree dans parse_andor()\n");
	pipeline = parse_pipeline(cur);
	if (!pipeline)
	{
		fprintf(stderr, "[debuger] Aucun pipeline trouve\n");
		return NULL;
	}
	head = init_andor(pipeline);
	while (*cur && ((*cur)->type == AND_IF || (*cur)->type == OR_IF))
	{
		fprintf(stderr, "[debuger] Operateur logique trouve : \"%s\"\n", (*cur)->value);
		op = (*cur)->type;
		*cur = (*cur)->next;
		next_pipeline = parse_pipeline(cur);
		if (!next_pipeline)
		{
			fprintf(stderr, "[debuger] Erreur: aucun pipeline après l'operateur\n");
			break;
		}
		node = init_andor(next_pipeline);
		if (!node)
			break;
		node->op = op;
		append_andor(head, node);
	}
	fprintf(stderr, "[debuger] Fin de parse_andor()\n");
	return head;
}

void append_andor(t_andor *head, t_andor *node)
{
	t_andor *cur;

	cur = head;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

t_andor *init_andor(t_command *pipeline)
{
	t_andor *andor;

	andor = malloc(sizeof(t_andor));
	if (!andor)
	{ perror("malloc"); return NULL; }
	andor->pipeline = pipeline;
	andor->op = 0;
	andor->next = NULL;
	return andor;
}
