/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:21:31 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/30 13:47:42 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

size_t	skip_whitespace(const char *input, size_t i)
{
	while (input[i] && is_whitespace(input[i]))
		i++;
	return (i);
}

bool	ensure_buffer_capacity(t_buffer *buf, size_t additional_len)
{
	size_t	required_cap;
	size_t	new_cap;
	char	*new_str;

	required_cap = buf->index + additional_len + 1;
	if (required_cap > buf->cap)
	{
		new_cap = buf->cap + additional_len + 1024;
		new_str = ft_realloc(buf->str, buf->cap, new_cap);
		if (!new_str)
		{
			perror("minishell: ft_realloc in append_to_buffer");
			free(buf->str);
			buf->str = NULL;
			buf->cap = 0;
			buf->index = 0;
			return (false);
		}
		buf->str = new_str;
		buf->cap = new_cap;
	}
	return (true);
}

void	append_to_buffer(t_buffer *buf, const char *s)
{
	size_t	len;

	if (!buf || !buf->str || !s)
		return ;
	len = ft_strlen(s);
	if (len == 0)
		return ;
	if (!ensure_buffer_capacity(buf, len))
		return ;
	ft_memcpy(buf->str + buf->index, s, len);
	buf->index += len;
	buf->str[buf->index] = '\0';
}
