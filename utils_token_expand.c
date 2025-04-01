/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:18:27 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/31 16:48:07 by michel           ###   ########.fr       */
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
		printf("Token expandi0 : %s\n", token->value);
		var_name = token->value + 1;
		val = env_get(env, var_name);
		free(token->value);
		if (val)
		{
			printf("Token expandi1 : %s\n", token->value);
			token->value = ft_strdup(val);
		}
		else
		{
			printf("Token expandi2 : %s\n", token->value);
			token->value = ft_strdup("");
		}
		printf("Token expandi : %s\n", token->value);
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

int	handle_no_command_arguments(t_command *cmd, int saved_stdin, int saved_stdout)
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
