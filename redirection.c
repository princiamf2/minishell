/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:29:58 by michel            #+#    #+#             */
/*   Updated: 2025/05/19 18:07:35 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_input_redirection(t_command *cmd)
{
	int	fd;

	if (!cmd->input)
		return (0);
	fd = open(cmd->input, O_RDONLY);
	if (fd < 0)
	{
		perror("minishell: open input");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("minishell: dup2 input");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_output_trunc_redirection(t_command *cmd)
{
	int	fd;

	if (!cmd->output)
		return (0);
	fd = open(cmd->output, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell: open output");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("minishell: dup2 output");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_output_append_redirection(t_command *cmd)
{
	int	fd;

	if (!cmd->output)
		return (0);
	fd = open(cmd->output, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror("minishell: open output (append)");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("minishell: dup2 output (append)");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_heredoc(t_command *cmd)
{
	int	fd;

	printf("DEBUG heredoc_tmp = %p \"%s\"\n", cmd->heredoc_tmp, cmd->heredoc_tmp);
	fd = open(cmd->heredoc_tmp, O_RDONLY);
	if (fd < 0)
	{
		perror("open heredoc");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		perror("dup2 heredoc");
		close(fd);
		return (-1);
	}
	close(fd);
	unlink(cmd->heredoc_tmp);
	free(cmd->heredoc_tmp);
	return (0);
}

int	handle_redirection(t_command *cmd)
{
	int	ret;

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
