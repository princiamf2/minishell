/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:30:47 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/11 17:16:07 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
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

typedef struct s_redir
{
	t_token_type		type;
	char				*target;
	bool				delim_quoted;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**args;
	char				*input;
	char				*output;
	int					append;
	int					heredoc;
	bool				subshell;
	struct s_redir		*redirs;
	struct s_command	*next_pipe;
	struct s_cmdlist	*subshell_ast;
}						t_command;

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
}						t_token_type;

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	bool				quoted;
	struct s_token		*next;
}						t_token;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_buffer
{
	char				*str;
	size_t				index;
	size_t				cap;
}						t_buffer;

typedef struct s_token_state
{
	const char			*input;
	size_t				i;
	bool				in_single;
	bool				in_double;
	char				*buffer;
	size_t				buf_index;
}						t_token_state;

typedef struct s_andor
{
	t_command			*pipeline;
	t_token_type		op;
	struct s_andor		*next;
}						t_andor;

typedef struct s_cmdlist
{
	t_andor				*andor;
	struct s_cmdlist	*next;
}						t_cmdlist;

char					*ft_strtok(char *s, const char *delim);
char					*find_excutable(const char *cmd);
int						execute_command(char **args, char **envp);
int						ft_strcmp(char *s1, const char *s2);
int						is_builtins(char *cmd);
int						execute_builtin(char **args, char **envp);
int						builtin_echo(char **args);
int						builtin_pwd(char **args);
int						builtin_export(char **args, char ***env);
int						builtin_unset(char **args, char ***env);
int						builtin_env(char **env);
int						builtin_cd(char **args);
int						find_env_index(char **env, char *var);
char					**add_env_variable(char **env, char *new_var);
int						find_env_index_unset(char **env, char *var);
int						handle_input_redirection(t_command *cmd);
int						handle_output_trunc_redirection(t_command *cmd);
int						handle_output_append_redirection(t_command *cmd);
int						handle_heredoc(t_command *cmd);
int						handle_redirection(t_command *cmd);
char					*generate_tmp_name(void);
int						open_tmp_heredoc_file(char **tmp_name);
int						read_and_write_heredoc(int fd, const char *delimiter);
int						finalize_heredoc(int fd, char *tmp_name);
t_token					*create_token(const char *str);
bool					is_whitespace(char c);
size_t					skip_whitespace(const char *input, size_t i);
void					handle_dollar_question(const char *input, size_t *i,
							char *buffer, size_t *index);
void					handle_dollar_variable(const char *input, size_t *i,
							char *buffer, size_t *index);
char					*collect_token(t_token_state *state);
void					process_token_char(t_token_state *state);
t_token					*lexer(const char *input);
t_cmdlist				*build_subshell_ast(t_token *tokens);
t_token					*extract_subshell_tokens(t_token **cur);
t_command				*create_subshell_command(t_cmdlist *sub_ast);
int						parse_command_arguments(t_command *cmd, t_token **cur);
void					handle_word_token(t_token **cur, t_command *cmd,
							size_t *argc, size_t *capacity);
void					add_argument_to_cmd(t_command *cmd, const char *arg,
							size_t *argc, size_t *capacity);
void					parse_redirection(t_command *cmd, t_token **cur);
t_andor					*parse_andor(t_token **cur);
void					append_andor(t_andor *head, t_andor *node);
t_andor					*init_andor(t_command *pipeline);
t_command				*init_command(void);
t_cmdlist				*parse_line(t_token *tokens);
bool					skip_opening_paren(t_token **cur);
char					**glob_pattern(const char *pattern);
char					*read_heredoc(const char *delim, bool quoted);
t_command				*parse_pipeline(t_token **cur);
void					get_dir_and_pattern(const char *pattern, char *dirpath,
							size_t size, const char **pat);
char					**read_directory_matches(const char *dirpath,
							const char *pat);
void					append_line(char **content, size_t *len, size_t *cap,
							const char *line);

#endif
