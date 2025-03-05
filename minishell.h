/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:30:47 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/05 19:11:45 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

typedef struct s_command
{
	char			**args;
	char			*input;
	char			*output;
	int				append;
}					t_command;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_OPERATOR,
	TOKEN_REDIRECTION,
	TOKEN_WILDCARD,

}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
}					t_token;

char				*ft_strtok(char *s, const char *delim);
char				*find_excutable(const char *cmd);
int					execute_command(char **args, char **envp);
int					ft_strcmp(char *s1, char *s2);
#endif
