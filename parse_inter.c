/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_inter.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 16:35:28 by nicolsan          #+#    #+#             */
/*   Updated: 2025/04/30 13:47:28 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*interpret_raw_value(const char *raw_value)
{
	t_interp_params	params;
	char			*result;
	size_t			result_final_idx;

	if (!raw_value)
		return (NULL);
	get_interp_params(raw_value, &params);
	result = malloc(params.len + 1);
	if (!result)
		return (NULL);
	result_final_idx = interpret_and_copy(result, raw_value, params);
	result[result_final_idx] = '\0';
	return (result);
}

void	get_interp_params(const char *raw_value, t_interp_params *params)
{
	params->len = ft_strlen(raw_value);
	params->quote_char = 0;
	params->start = 0;
	params->end = params->len;
	if (params->len >= 2)
	{
		if (raw_value[0] == '\'' && raw_value[params->len - 1] == '\'')
		{
			params->quote_char = '\'';
			params->start = 1;
			params->end = params->len - 1;
		}
		else if (raw_value[0] == '\"' && raw_value[params->len - 1] == '\"')
		{
			params->quote_char = '\"';
			params->start = 1;
			params->end = params->len - 1;
		}
	}
}

void	process_char_for_interpretation(t_interp_ctx *ctx)
{
	char	current_char;
	char	next_char;

	current_char = ctx->raw[*(ctx->i_ptr)];
	if (current_char == '\\' && (*(ctx->i_ptr) + 1) < ctx->params.end)
	{
		next_char = ctx->raw[*(ctx->i_ptr) + 1];
		if (ctx->params.quote_char == '\"')
		{
			if (next_char == '\\' || next_char == '$' || next_char == '\"')
			{
				ctx->res[(*(ctx->res_idx_ptr))++] = next_char;
				(*(ctx->i_ptr))++;
			}
			else
				ctx->res[(*(ctx->res_idx_ptr))++] = current_char;
		}
		else
		{
			ctx->res[(*(ctx->res_idx_ptr))++] = next_char;
			(*(ctx->i_ptr))++;
		}
	}
	else
		ctx->res[(*(ctx->res_idx_ptr))++] = current_char;
}

size_t	interpret_and_copy(char *result, const char *raw_value,
		t_interp_params params)
{
	t_interp_ctx	ctx;
	size_t			i;
	size_t			result_idx;

	i = params.start;
	result_idx = 0;
	ctx.raw = raw_value;
	ctx.res = result;
	ctx.i_ptr = &i;
	ctx.res_idx_ptr = &result_idx;
	ctx.params = params;
	while (*(ctx.i_ptr) < ctx.params.end)
	{
		if (ctx.params.quote_char == '\'')
			ctx.res[(*(ctx.res_idx_ptr))++] = ctx.raw[*(ctx.i_ptr)];
		else
			process_char_for_interpretation(&ctx);
		(*(ctx.i_ptr))++;
	}
	return (*(ctx.res_idx_ptr));
}
