/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_memo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:11:58 by nicolsan          #+#    #+#             */
/*   Updated: 2025/05/19 19:40:41 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"

char	*read_heredoc(const char *delim, bool quoted)
{
	size_t	cap;
	size_t	len;
	char	*content;

	(void)quoted;
	cap = 1024;
	len = 0;
	content = malloc(cap);
	if (!content)
		exit(1);
	content[0] = '\0';
	while (!process_heredoc_line(delim, &content, &len, &cap))
		;
	return (content);
}

bool	process_heredoc_line(const char *delim, char **content, size_t *len,
		size_t *cap)
{
	char	*line;

	line = readline("heredoc> ");
	if (g_exit_status == 130)
	{
		free(line);
		return (true);
	}
	if (ft_strcmp(line, delim) == 0)
	{
		free(line);
		return (1);
	}
	append_line(content, len, cap, line);
	free(line);
	return (0);
}

void	append_line(char **content, size_t *len, size_t *cap, const char *line)
{
	size_t	l_len;

	if (!content || !line)
	{
		return ;
	}
	l_len = ft_strlen(line);
	ensure_content_capacity(content, *len, cap, l_len);
	if (!*content)
	{
		return ;
	}
	ft_memcpy(*content + *len, line, l_len);
	*len += l_len;
	(*content)[(*len)++] = '\n';
	(*content)[*len] = '\0';
}

void	ensure_content_capacity(char **content_ptr, size_t current_len,
		size_t *cap_ptr, size_t line_len)
{
	size_t	required_cap;
	size_t	old_size;
	size_t	new_cap;
	char	*tmp;

	required_cap = current_len + line_len + 2;
	if (required_cap > *cap_ptr)
	{
		old_size = *cap_ptr;
		new_cap = *cap_ptr + line_len + 1024;
		tmp = ft_realloc(*content_ptr, old_size, new_cap);
		if (!tmp)
		{
			perror("minishell: ft_realloc in append_line");
			free(*content_ptr);
			*content_ptr = NULL;
			exit(1);
		}
		*content_ptr = tmp;
		*cap_ptr = new_cap;
	}
}

void	install_heredoc_signals(void)
{
	struct sigaction	act;

	rl_catch_signals = 0;
	act.sa_handler = sigint_heredoc_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);
	act.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &act, NULL);
}
