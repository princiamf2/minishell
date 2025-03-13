/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils9.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:58:02 by michel            #+#    #+#             */
/*   Updated: 2025/03/12 22:09:20 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *build_fullpath(const char *dirpath, const char *filename)
{
    size_t len_dir;
    size_t len_file;
    char *fullpath;
    
	if (ft_strcmp(dirpath, ".") == 0)
		return ft_strdup(filename);
    len_dir = ft_strlen(dirpath);
	len_file = ft_strlen(filename);
	fullpath = malloc(len_dir + 1 + len_file + 1);
	if (!fullpath)
		return NULL;
	ft_strcpy(fullpath, dirpath);
	fullpath[len_dir] = '/';
	ft_strcpy(fullpath + len_dir + 1, filename);
	return fullpath;
}

char *ft_strcpy(char *dest, const char *src)
{
    int i = 0;
    while (src[i] != '\0')
    {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
    return dest;
}

void handle_sigint(int sig)
{
	(void)sig;
	fprintf(stderr, "\n[debuger] SIGINT reçu, prompt reaffiche.\n");
	fflush(stderr);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void handle_sigquit(int sig)
{
	(void)sig;
}

void free_tokens(t_token *tokens)
{
    t_token *tmp;
    
    while (tokens)
    {
        tmp = tokens->next;
        free(tokens->value);
        free(tokens);
        tokens = tmp;
    }
}
