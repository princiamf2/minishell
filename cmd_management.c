/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils11.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:24:42 by michel            #+#    #+#             */
/*   Updated: 2025/04/07 13:28:46 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_command(t_command *cmd)
{
	t_command	*next_pipe_to_free;

	if (!cmd)
		return ;
	next_pipe_to_free = cmd->next_pipe;
	if (!cmd->subshell)
		free_command_args(cmd);
	free_command_redirs(cmd);
	if (cmd->input)
		free(cmd->input);
	if (cmd->output)
		free(cmd->output);
	if (cmd->subshell && cmd->subshell_ast)
	{
		free_cmdlist(cmd->subshell_ast);
	}
	free(cmd);
	if (next_pipe_to_free)
		free_command(next_pipe_to_free);
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
