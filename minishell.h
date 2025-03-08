/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:30:47 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/08 20:29:20 by michel           ###   ########.fr       */
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
int					is_builtins(char *cmd);
int					execute_builtin(char **args, char **envp);
int					builtin_echo(char **args);
int					builtin_pwd(char **args);
int					builtin_export(char **args, char ***env);
int					builtin_unset(char **args, char ***env);
int					builtin_env(char **env);
int					builtin_cd(char **args);
int					find_env_index(char **env, char *var);
char				**add_env_variable(char **env, char *new_var);
int					find_env_index_unset(char **env, char *var);

#endif
