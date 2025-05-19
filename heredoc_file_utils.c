/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:54:09 by mm-furi           #+#    #+#             */
/*   Updated: 2025/05/19 19:30:30 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*prepare_delim(char *raw, int quoted)
{
	char	*tmp;
	char	*r;

	if (!raw)
		return (NULL);
	tmp = ft_strdup(raw);
	if (!tmp)
		return (NULL);
	if (!quoted)
	{
		r = interpret_raw_value(tmp);
		free(tmp);
		tmp = r;
		if (!tmp)
			return (NULL);
	}
	return (tmp);
}

int	loop_heredoc(int fd, char *delim, int quoted)
{
	char	*line;

	(void)(quoted);
	while (1)
	{
		line = readline("heredoc> ");
		if (g_exit_status == SIGINT)
		{
			free(line);
			return (SIGINT);
		}
		if (!line)
			return (0);
		if (!strcmp(line, delim))
		{
			free(line);
			return (0);
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

int	cleanup_heredoc(int fd, int status, char *tmpname)
{
	if (close(fd) < 0)
		return (-1);
	if (status == SIGINT)
	{
		unlink(tmpname);
		free(tmpname);
		return (SIGINT);
	}
	if (status < 0)
		return (-1);
	return (0);
}
