/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:34:47 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/13 12:12:17 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command *init_command(void)
{
	t_command *cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc");
		return NULL;
	}
	cmd->subshell = false;
	cmd->subshell_ast = NULL;
	cmd->redirs = NULL;
	cmd->next_pipe = NULL;
	return cmd;
}

bool skip_opening_paren(t_token **cur)
{
	if (!cur || !*cur || ft_strcmp((*cur)->value, "(") != 0)
	{
		fprintf(stderr, "[debuger] Erreur: pas de '(' pour subshell\n");
		return false;
	}
	*cur = (*cur)->next;
	return true;
}

t_token *collect_tokens_until_closing(t_token **cur)
{
	t_token *start;
	t_token *prev;
	int balance;

	start = *cur;
	prev = NULL;
	balance = 1;
	while (*cur)
	{
		if (ft_strcmp((*cur)->value, "(") == 0)
			balance++;
		else if (ft_strcmp((*cur)->value, ")") == 0)
		{
			balance--;
			if (balance == 0)
				break;
		}
		prev = *cur;
		*cur = (*cur)->next;
	}
	if (!*cur || balance != 0)
	{
		fprintf(stderr, "[debuger] Erreur: parenthèses non equilibrees\n");
		return NULL;
	}
	if (prev)
		prev->next = NULL;
	return start;
}

char **glob_pattern(const char *pattern)
{
	if (!pattern)
		return NULL;
	char dirpath[PATH_MAX];
	const char *pat;
	get_dir_and_pattern(pattern, dirpath, sizeof(dirpath), &pat);
	return read_directory_matches(dirpath, pat);
}
