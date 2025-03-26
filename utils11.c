/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils11.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:24:42 by michel            #+#    #+#             */
/*   Updated: 2025/03/26 19:34:57 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command(t_command *cmd)
{
	int			i;
	t_redir		*redir;
	t_command	*next_pipe;

	if (!cmd)
		return ;
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			free(cmd->args[i]);
			i++;
		}
		free(cmd->args);
	}
	while (cmd->redirs)
	{
		redir = cmd->redirs;
		cmd->redirs = cmd->redirs->next;
		if (redir->target)
			free(redir->target);
		free(redir);
	}
	next_pipe = cmd->next_pipe;
	free(cmd);
	free_command(next_pipe);
}

int	count_commands(t_command *pipeline)
{
	int	count;

	count = 0;
	while (pipeline)
	{
		count++;
		pipeline = pipeline->next_pipe;
	}
	return (count);
}

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc");
		return (NULL);
	}
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->subshell = false;
	cmd->redirs = NULL;
	cmd->next_pipe = NULL;
	cmd->subshell_ast = NULL;
	return (cmd);
}

int	update_env_var(t_data *data, char *key, char *value)
{
	t_env	*node;
	size_t	key_len;

	node = data->env;
	key_len = ft_strlen(key);
	while (node)
	{
		if (ft_strncmp(node->key, key, key_len) == 0
			&& node->key[key_len] == '\0')
		{
			free(node->value);
			node->value = ft_strdup(value);
			return (1);
		}
		node = node->next;
	}
	return (0);
}

int	add_env_var(t_data *data, char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		perror("malloc");
		free(key);
		free(value);
		return (1);
	}
	new_node->key = key;
	new_node->value = value;
	new_node->next = data->env;
	data->env = new_node;
	return (0);
}
