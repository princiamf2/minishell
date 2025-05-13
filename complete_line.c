/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   complete_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 14:48:45 by mm-furi           #+#    #+#             */
/*   Updated: 2025/05/13 16:20:18 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	update_flags(const char *s, int *in_single, int *in_double)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == '\'' && !*in_double)
			*in_single ^= 1;
		else if (s[i] == '\"' && !*in_single)
			*in_double ^= 1;
		i++;
	}
}

static char	*append_next_line(char *line, char *more)
{
	char	*tmp;
	char	*new_line;

	tmp = ft_strjoin(line, "\n");
	free(line);
	new_line = ft_strjoin(tmp, more);
	free(tmp);
	free(more);
	return (new_line);
}

char	*read_complete_line(void)
{
	char	*line;
	char	*more;
	int		in_single;
	int		in_double;

	*((char **)&rl_basic_quote_characters) = "";
	in_single = 0;
	in_double = 0;
	line = readline("minishell$> ");
	if (!line)
		return (NULL);
	update_flags(line, &in_single, &in_double);
	while (in_single || in_double)
	{
		more = readline("dquotes> ");
		if (!more)
			break ;
		line = append_next_line(line, more);
		in_single = 0;
		in_double = 0;
		update_flags(line, &in_single, &in_double);
	}
	return (line);
}
