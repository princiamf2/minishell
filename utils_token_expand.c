/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_token_expand.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 16:18:27 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/27 16:47:27 by mm-furi          ###   ########.fr       */
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
