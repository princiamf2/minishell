/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expand.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:54:53 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:52:31 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_token(t_token *token, t_env *env, t_env *local_vars)
{
	char	*var_name;
	char	*val;

	if (token->value[0] == '$')
	{
		var_name = token->value + 1;
		val = env_get(env, var_name);
		if (!val)
			val = env_get(local_vars, var_name);
		free(token->value);
		if (val)
			token->value = ft_strdup(val);
		else
			token->value = ft_strdup("");
	}
}

void	expand_tokens(t_token *tokens, t_env *env, t_env *local_vars)
{
	while (tokens)
	{
		expand_token(tokens, env, local_vars);
		tokens = tokens->next;
	}
}
