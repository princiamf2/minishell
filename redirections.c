/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:29:58 by michel            #+#    #+#             */
/*   Updated: 2025/03/09 03:37:58 by michel           ###   ########.fr       */
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
	int fd;
    char *tmp_name;
    int ret;
    
    if (!cmd->heredoc)
        return (0);
    fd = open_tmp_heredoc_file(&tmp_name);
    if (fd < 0)
    {
        perror("open heredoc");
        free(tmp_name);
        return (-1);
    }
    ret = read_and_write_heredoc(fd, cmd->input);
    if (ret < 0)
    {
        close(fd);
        free(tmp_name);
        return (-1);
    }
    ret = finalize_heredoc(fd, tmp_name);
    return (ret);
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
