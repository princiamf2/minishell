/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:30:47 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/10 17:46:33 by mm-furi          ###   ########.fr       */
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
# include <stdbool.h>
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
	int				heredoc;
}					t_command;

typedef enum e_token_type
{
	WORD,
	PIPE,
	SEMICOLON,
	AND_IF,
	OR_IF,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC,
	PAREN_OPEN,
	PAREN_CLOSE
}					t_token_type;

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	bool			quoted;
	struct s_token	*next;
}					t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_buffer
{
	char			*str;
	size_t			index;
	size_t			cap;
}					t_buffer;

typedef struct s_token_state
{
	const char		*input;
	size_t			i;
	bool			in_single;
	bool			in_double;
	char			*buffer;
	size_t			buf_index;
}					t_token_state;

char				*ft_strtok(char *s, const char *delim);
char				*find_excutable(const char *cmd);
int					execute_command(char **args, char **envp);
int					ft_strcmp(char *s1, const char *s2);
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
int					handle_input_redirection(t_command *cmd);
int					handle_output_trunc_redirection(t_command *cmd);
int					handle_output_append_redirection(t_command *cmd);
int					handle_heredoc(t_command *cmd);
int					handle_redirection(t_command *cmd);
char				*generate_tmp_name(void);
int					open_tmp_heredoc_file(char **tmp_name);
int					read_and_write_heredoc(int fd, const char *delimiter);
int					finalize_heredoc(int fd, char *tmp_name);
t_token				*create_token(const char *str);
bool				is_whitespace(char c);
size_t				skip_whitespace(const char *input, size_t i);
void				handle_dollar_question(const char *input, size_t *i,
						char *buffer, size_t *index);
void				handle_dollar_variable(const char *input, size_t *i,
						char *buffer, size_t *index);
char				*collect_token(t_token_state *state);
void				process_token_char(t_token_state *state);
t_token				*lexer(const char *input);

#endif
