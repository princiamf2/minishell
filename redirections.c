/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:29:58 by michel            #+#    #+#             */
/*   Updated: 2025/03/09 01:40:24 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_input_redirection(t_command *cmd)
{
    int fd;

    if (!cmd->input)
        return (0);
    fd = open(cmd->input, O_RDONLY);
    if (fd < 0)
    {
        perror("open input");
        return (-1);
    }
    if (dup2(fd, STDIN_FILENO) < 0)
    {
        perror("dub2 input");
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

int handle_output_trunc_redirection(t_command *cmd)
{
    int fd;
    
    if (!cmd->output)
        return (0);
    fd = open(cmd->output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("open output");
        return (-1);
    }
    if (dup2(fd, STDOUT_FILENO) < 0)
    {
        perror("dup2 output");
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

int handle_output_append_redirection(t_command *cmd)
{
    int fd;
    
    if (!cmd->output)
        return (0);
    fd = open(cmd->output, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror("open output (append)");
        return (-1);
    }
    if (dup2(fd, STDOUT_FILENO) < 0)
    {
        perror("dup2 output (append)");
        close(fd);
        return (-1);
    }
    close(fd);
    return (0);
}

int handle_heredoc(t_command *cmd)
{
	char	*line;
	int		fd;
	char	tmp[] = "/tmp/minishell_heredocXXXXXX";

	if (!cmd->heredoc)
		return (0);
	fd = mkstemp(tmp);
	if (fd < 0)
	{
		perror("mkstemp");
		return (-1);
	}
	unlink(tmp);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, cmd->input) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 heredoc");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int handle_redirection(t_command *cmd)
{
	int ret;

	if (cmd->heredoc)
	{
		ret = handle_heredoc(cmd);
		if (ret < 0)
			return (ret);
	}
	else if (cmd->input)
	{
		ret = handle_input_redirection(cmd);
		if (ret < 0)
			return (ret);
	}
	if (cmd->output)
	{
		if (cmd->append)
			ret = handle_output_append_redirection(cmd);
		else
			ret = handle_output_trunc_redirection(cmd);
		if (ret < 0)
			return (ret);
	}
	return (0);
}
