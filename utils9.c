/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils9.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:58:02 by michel            #+#    #+#             */
/*   Updated: 2025/04/04 14:49:21 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "global_vars.h"

char	*build_fullpath(const char *dirpath, const char *filename)
{
	size_t	len_dir;
	size_t	len_file;
	char	*fullpath;

	if (ft_strcmp(dirpath, ".") == 0)
		return (ft_strdup(filename));
	len_dir = ft_strlen(dirpath);
	len_file = ft_strlen(filename);
	fullpath = malloc(len_dir + 1 + len_file + 1);
	if (!fullpath)
		return (NULL);
	ft_strcpy(fullpath, dirpath);
	fullpath[len_dir] = '/';
	ft_strcpy(fullpath + len_dir + 1, filename);
	return (fullpath);
}

char	*ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	handle_sigint(int sig)
{
	(void)sig;
	rl_on_new_line();
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_redisplay();
	g_exit_status = 130;
}

void	handle_sigquit(int sig)
{
	(void)sig;
}

void	free_tokens(t_token *tokens)
{
	t_token	*tmp;

	while (tokens)
	{
		tmp = tokens->next;
		if (tokens->value)
			free(tokens->value);
		free(tokens);
		tokens = tmp;
	}
}
