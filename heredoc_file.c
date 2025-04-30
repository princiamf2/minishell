/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 02:03:50 by michel            #+#    #+#             */
/*   Updated: 2025/04/30 14:00:23 by nicolsan         ###   ########.fr       */
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
	return (fd);
}

bool	process_heredoc_input_line(int fd, const char *interpreted_delimiter)
{
	char	*line;
	bool	should_break;

	should_break = false;
	line = readline("herdoc> ");
	if (!line)
	{
		should_break = true;
	}
	else
	{
		if (ft_strcmp(line, interpreted_delimiter) == 0)
		{
			should_break = true;
		}
		else
		{
			write(fd, line, ft_strlen(line));
			write(fd, "\n", 1);
		}
		free(line);
	}
	return (should_break);
}

int	read_and_write_heredoc(int fd, const char *raw_delimiter)
{
	char	*interpreted_delimiter;

	interpreted_delimiter = interpret_raw_value(raw_delimiter);
	if (!interpreted_delimiter)
	{
		perror("minishell: interpret_raw_value failed for heredoc delimiter");
		return (-1);
	}
	while (true)
	{
		if (process_heredoc_input_line(fd, interpreted_delimiter))
			break ;
	}
	free(interpreted_delimiter);
	return (0);
}

int	finalize_heredoc(int fd, char *tmp_name)
{
	int	ret;
	int	final_ret;

	close(fd);
	fd = open(tmp_name, O_RDONLY);
	if (fd < 0)
	{
		perror("open heredoc (finalize)");
		free(tmp_name);
		return (-1);
	}
	ret = dup2(fd, STDIN_FILENO);
	close(fd);
	unlink(tmp_name);
	free(tmp_name);
	if (ret >= 0)
		final_ret = 0;
	else
		final_ret = -1;
	return (final_ret);
}
