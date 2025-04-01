/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:18:27 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/01 17:09:33 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	process_assignement(char *input, t_data *data)
{
	char	*eq;

	eq = ft_strchr(input, '=');
	if (!eq)
		return ;
	*eq = '\0';
	env_set(&(data->env), input, eq + 1);
	*eq = '=';
}

void	expand_token(t_token *token, t_env *env)
{
	char	*var_name;
	char	*val;

	if (token->value && token->value[0] == '$')
	{
		var_name = token->value + 1;
		val = env_get(env, var_name);
		free(token->value);
		if (val)
			token->value = ft_strdup(val);
		else
			token->value = ft_strdup("");
	}
}

void	expand_tokens(t_token *tokens, t_env *env)
{
	while (tokens)
	{
		expand_token(tokens, env);
		tokens = tokens->next;
	}
}

int	handle_no_command_arguments(t_command *cmd, int saved_stdin,
		int saved_stdout)
{
	if (!cmd->args || !cmd->args[0])
	{
		if (handle_redirection(cmd) < 0)
		{
			close(saved_stdout);
			return (1);
		}
		restore_stdin(saved_stdin);
		if (dup2(saved_stdout, STDOUT_FILENO) < 0)
			perror("dup2 stdout");
		close(saved_stdout);
		return (0);
	}
	return (-1);
}

void	process_line(t_data *data)
{
	if (*data->input)
		add_history(data->input);
	if (ft_strchr(data->input, '=') != NULL)
	{
		process_assignement(data->input, data);
		free(data->input);
		return ;
	}
	data->tokens = lexer(data->input, data->env);
	if (!data->tokens)
		return ;
	if (!validate_tokens_adv(data->tokens))
	{
		ft_putendl_fd("Syntaxe invalide.\n", 2);
		free_tokens(data->tokens);
		return ;
	}
	data->cmdlist = parse_line(data->tokens);
	free_tokens(data->tokens);
	if (!data->cmdlist)
		return ;
	data->exit_status = execute_cmdlist(data->cmdlist, data);
	free_cmdlist(data->cmdlist);
	free(data->input);
}
