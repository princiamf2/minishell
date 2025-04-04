/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lex3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:19:44 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/04 16:59:29 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_dollar_sign(t_token_state *state, int exit_status, t_env *env)
{
	if (state->input[state->i] == '$' && !state->in_single)
	{
		if (state->input[state->i + 1] == '?')
			handle_dollar_question(state->input, &state->i, state->buffer,
				exit_status);
		else
			handle_dollar_variable(state->input, &state->i, state->buffer, env);
	}
}

void	process_regular_character(t_token_state *state)
{
	char	c;

	if (!state->input[state->i])
		return ;
	c = state->input[state->i];
	append_to_buffer(state->buffer, (char[]){c, '\0'});
	state->i++;
}

void	process_token_char(t_token_state *state, int exit_status, t_env *env)
{
	if (!state->input[state->i])
		return ;
	handle_whitespace(state);
	handle_quotes(state);
	handle_escape_character(state);
	handle_dollar_sign(state, exit_status, env);
	process_regular_character(state);
}

int	is_option_n(const char *str)
{
	int	i;

	if (str[0] != '-')
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

char	*get_first_token(const char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != ' ' && input[i] != '\t')
		i++;
	return (ft_substr(input, 0, i));
}
