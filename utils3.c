/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 16:21:31 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/13 12:06:33 by michel           ###   ########.fr       */
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

void append_to_buffer(t_buffer *buf, const char *s)
{
    size_t len;
	size_t new_cap;
	char *new_str;

	len = ft_strlen(s);
    /* Si nécessaire, réallouer pour assurer la capacité */
    if (buf->index + len + 1 > buf->cap)
    {
        new_cap = buf->cap + len + 1024;
        new_str = realloc(buf->str, new_cap);
        if (!new_str)
        {
            free(buf->str);
            buf->str = NULL;
            buf->cap = 0;
            return;
        }
        buf->str = new_str;
        buf->cap = new_cap;
    }
    ft_strlcpy(buf->str + buf->index, s, len + 1);
    buf->index += len;
}

void	handle_dollar_question(const char *input, size_t *i, t_buffer *buf, int exit_status)
{
	char *num_str;

	(void)input;
	num_str = ft_itoa(exit_status);
	if (!num_str)
		return;
	append_to_buffer(buf, num_str);
	free(num_str);
	*i += 2;
}

void	handle_dollar_variable(const char *input, size_t *i, t_buffer *buf, t_env *env)
{
	size_t start;
	size_t len;
	char var[256];
	char *val;

	start = *i + 1;
	len = 0;
	while (input[start + len] &&
		   (isalnum(input[start + len]) || input[start + len] == '_'))
		len++;
	if (len)
	{
		if (len >= sizeof(var))
			len = sizeof(var) - 1;
		ft_strncpy(var, input + start, len);
		var[len] = '\0';
		val = env_get(env, var);
		if (!val)
			val = "";
		append_to_buffer(buf, val);
		*i = start + len;
	}
	else
		buf->str[buf->index++] = input[(*i)++];
}
