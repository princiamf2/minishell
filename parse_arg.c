/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:50:12 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:55:01 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_cmd_args(t_command *cmd, size_t *capacity, size_t *argc)
{
	*capacity = 8;
	*argc = 0;
	cmd->args = malloc(*capacity * sizeof(char *));
	if (!cmd->args)
	{
		perror("malloc init_cmd_args");
		return (-1);
	}
	cmd->args[0] = NULL;
	return (0);
}

void	collect_cmd_args(t_command *cmd, t_token **cur, size_t *argc,
		size_t *capacity)
{
	while (*cur && (((*cur)->type == WORD) || is_redirection(*cur)))
	{
		if ((*cur)->type == WORD)
			handle_word_token(cur, cmd, argc, capacity);
		else
			parse_redirection(cmd, cur);
	}
}

void	handle_word_token(t_token **cur, t_command *cmd, size_t *argc,
		size_t *capacity)
{
	if (strchr((*cur)->value, '*') && !(*cur)->quoted)
	{
		if (try_expand_glob(cur, cmd, argc, capacity))
			return ;
	}
	add_argument_to_cmd(cmd, (*cur)->value, argc, capacity);
	*cur = (*cur)->next;
}

bool	ensure_args_capacity(t_command *cmd, size_t argc, size_t *capacity)
{
	char	**tmp;
	size_t	old_size;
	size_t	new_capacity;

	if (argc + 1 >= *capacity)
	{
		old_size = *capacity * sizeof(char *);
		if (*capacity == 0)
			new_capacity = 8;
		else
			new_capacity = *capacity * 2;
		tmp = ft_realloc(cmd->args, old_size, new_capacity * sizeof(char *));
		if (!tmp)
		{
			perror("minishell: ft_realloc in ensure_args_capacity");
			return (false);
		}
		cmd->args = tmp;
		*capacity = new_capacity;
	}
	return (true);
}

void	add_argument_to_cmd(t_command *cmd, const char *raw_arg_value,
		size_t *argc, size_t *capacity)
{
	char	*final_arg;
	bool	args_ready;

	if (!cmd || !raw_arg_value || !argc || !capacity)
		return ;
	final_arg = interpret_raw_value(raw_arg_value);
	if (!final_arg)
	{
		perror("minishell: interpret_raw_value failed");
		return ;
	}
	if (!cmd->args)
		args_ready = (init_cmd_args(cmd, capacity, argc) == 0);
	else
		args_ready = ensure_args_capacity(cmd, *argc, capacity);
	if (!args_ready)
	{
		free(final_arg);
		return ;
	}
	cmd->args[*argc] = final_arg;
	(*argc)++;
	cmd->args[*argc] = NULL;
}
