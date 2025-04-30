/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_line.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:18:27 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 17:00:08 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_line(t_data *data)
{
	int	special_case_handled;

	special_case_handled = handle_special_input_cases(data);
	if (special_case_handled)
		return ;
	if (!lex_and_validate_input(data))
	{
		if (data->input)
		{
			free(data->input);
			data->input = NULL;
		}
		return ;
	}
	parse_execute_and_cleanup(data);
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
}

bool	lex_and_validate_input(t_data *data)
{
	data->tokens = lexer(data->input, data->env, data);
	if (!data->tokens)
	{
		return (false);
	}
	if (!validate_tokens_adv(data->tokens))
	{
		ft_putendl_fd("Syntaxe invalide.\n", 2);
		free_tokens(data->tokens);
		data->tokens = NULL;
		data->exit_status = 2;
		return (false);
	}
	return (true);
}

void	parse_execute_and_cleanup(t_data *data)
{
	data->cmdlist = parse_line(data->tokens);
	free_tokens(data->tokens);
	data->tokens = NULL;
	if (!data->cmdlist)
		return ;
	data->exit_status = execute_cmdlist(data->cmdlist, data);
	free_cmdlist(data->cmdlist);
	data->cmdlist = NULL;
}

static bool	is_assignment(const char *s)
{
	char	*eq;
	size_t	len;
	char	*key;

	eq = ft_strchr(s, '=');
	if (!eq || eq == s)
		return (false);
	len = eq - s;
	key = malloc(len + 1);
	if (!key)
		return (false);
	ft_strncpy(key, s, len);
	key[len] = '\0';
	if (!is_valid_identifier(key))
	{
		free(key);
		return (false);
	}
	free(key);
	return (true);
}

bool	handle_special_input_cases(t_data *data)
{
	if (data->input && is_assignment(data->input))
	{
		process_local_assignmment(data->input, data);
		free(data->input);
		data->input = NULL;
		return (true);
	}
	return (false);
}
