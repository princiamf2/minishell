/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 02:03:50 by michel            #+#    #+#             */
/*   Updated: 2025/03/14 17:55:42 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *generate_tmp_name(void)
{
    static int counter;
    char *prefix;
    char *num_str;
    char *tmp_name;
    static int init;

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

int open_tmp_heredoc_file(char **tmp_name)
{
    int fd;

    *tmp_name = generate_tmp_name();
    unlink(*tmp_name);
    fd = open(*tmp_name, O_WRONLY | O_CREAT | O_EXCL, 0600);
    return (fd);
}

int read_and_write_heredoc(int fd, const char *delimiter)
{
    char *line;

    while (1)
    {
        line = readline("> ");
        ft_putstr_fd("ligne lue: ", 1);
        ft_putstr_fd(line, 1);
        ft_putchar_fd('\n', 1);
        if (!line || ft_strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    return (0);
}

int finalize_heredoc(int fd, char *tmp_name)
{
    int ret;

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
    if (ret < 0)
        return (-1);
    return (0);
}
