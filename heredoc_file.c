/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 02:03:50 by michel            #+#    #+#             */
/*   Updated: 2025/05/19 19:50:37 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*generate_tmp_name(void)
{
	static int	counter;
	char		*prefix;
	char		*num_str;
	char		*tmp_name;
	static int	init;

	if (!init)
	{
		counter = 0;
		init = 1;
	}
	prefix = "/tmp/minishell_heredoc_";
	num_str = ft_itoa(counter);
	tmp_name = ft_strjoin(prefix, num_str);
	free(num_str);
	counter++;
	return (tmp_name);
}

int	open_tmp_heredoc_file(char **tmp_name)
{
	int	fd;

	*tmp_name = generate_tmp_name();
	unlink(*tmp_name);
	fd = open(*tmp_name, O_WRONLY | O_CREAT | O_EXCL, 0600);
	if (fd < 0)
	{
		free(*tmp_name);
		*tmp_name = NULL;
		return (-1);
	}
	return (fd);
}

int	process_heredoc_input_line(int fd, char *delim, int delim_quoted)
{
	char	*line;
	int		ret;
	char	*tmp;

	line = readline("heredoc> ");
	if (!line)
		return (SIGINT);
	if (!delim_quoted)
	{
		tmp = interpret_raw_value(line);
		free(line);
		line = tmp;
		if (!line)
			return (-1);
	}
	if (!ft_strcmp(line, delim))
	{
		free(line);
		return (0);
	}
	ret = write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	if (ret < 0)
		return (-1);
	return (1);
}

int	read_and_write_heredoc(char *raw_delim, int delim_quoted,
		char **heredoc_tmp)
{
	printf("rentre dans read_and_w\n");
	char	*delim;
	int		fd;
	int		status;

	delim = prepare_delim(raw_delim, delim_quoted);
	if (!delim)
		return (-1);
	fd = open_tmp_heredoc_file(heredoc_tmp);
	if (fd < 0)
	{
		free(delim);
		perror("heredoc");
		return (-1);
	}
	printf("DEBUG after open_tmp: *heredoc_tmp = %p \"%s\"\n",
       (void *)*heredoc_tmp, *heredoc_tmp);
	g_exit_status = 0;
	install_heredoc_signals();
	status = loop_heredoc(fd, delim, delim_quoted);
	restore_default_signals();
	free(delim);
	fprintf(stderr, "DEBUG loop status = %d\n", status);
	return (cleanup_heredoc(fd, status, *heredoc_tmp));
}
