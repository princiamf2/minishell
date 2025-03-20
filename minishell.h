/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:30:47 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/20 19:27:46 by michel           ###   ########.fr       */
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

typedef struct s_redir
{
	t_token_type		type;
	char				*target;
	bool				delim_quoted;
	struct s_redir		*next;
}						t_redir;

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
	t_buffer			*buffer;
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

typedef struct s_pipe_info
{
	int					n;
	int					*pipes;
}						t_pipe_info;

typedef struct s_data
{
	t_env				*env;
	t_cmdlist			*cmdlist;
	t_token				*tokens;
	t_pipe_info			*pipe;
	t_command			*cmd;
	char				*input;
	int					exit_status;
}						t_data;

char					*ft_strtok(char *s, const char *delim);
char					*find_excutable(const char *cmd);
int						execute_command(t_command *cmd, t_data *data);
int						ft_strcmp(const char *s1, const char *s2);
int						is_builtins(char *cmd);
int						execute_builtin(char **args, t_data *data);
int						builtin_echo(char **args);
int						builtin_pwd(char **args);
int						builtin_export(char **args, t_data *data);
int						builtin_unset(char **args, t_data *data);
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
							t_buffer *buf, int exit_status);
void					handle_dollar_variable(const char *input, size_t *i,
							t_buffer *buf, t_env *env);
char					*collect_token(t_token_state *state, int exit_status,
							t_env *env);
void					process_token_char(t_token_state *state,
							int exit_status, t_env *env);
t_token					*lexer(const char *input, t_env *env);
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
t_env					*env_init(char **envp);
char					*env_get(t_env *env, const char *key);
void					env_set(t_env **env, const char *key, const char *val);
void					env_unset(t_env **env, const char *key);
char					**env_to_array(t_env *env);
t_token					*collect_tokens_until_closing(t_token **cur);
bool					is_redirection(t_token *token);
t_command				*parse_command(t_token **cur);
t_command				*parse_subshell(t_token **cur);
char					*ft_strncpy(char *dest, const char *src, size_t n);
bool					match_pattern(const char *pattern, const char *str);
int						add_match(char ***matches, size_t *count,
							size_t *capacity, const char *dirpath,
							const char *filename);
char					*build_fullpath(const char *dirpath,
							const char *filename);
char					*ft_strcpy(char *dest, const char *src);
void					handle_sigint(int sig);
void					handle_sigquit(int sig);
bool					validate_tokens_adv(t_token *tokens);
void					free_tokens(t_token *tokens);
int						execute_cmdlist(t_cmdlist *commands, t_data *data);
int						execute_andor(t_andor *list, t_data *data);
t_command				*parse_pipeline(t_token **cur);
void					free_cmdlist(t_cmdlist *list);
void					free_andor(t_andor *andor);
void					free_command(t_command *cmd);
void					free_env(t_env *env);
int						count_commands(t_command *pipeline);
int						execute_pipeline(t_command *pipeline, t_data *data);
int						execute_full_command(t_command *cmd, t_data *data);
void					append_to_buffer(t_buffer *buf, const char *s);
void					print_env_array(char **env_array);
t_command				*init_command(void);
int						add_env_var(t_data *data, char *key, char *value);
int						update_env_var(t_data *data, char *key, char *value);
int						execute_builtin_with_redir(t_command *cmd,
							t_data *data);
int						handle_export_arguments(char **args, t_data *data);
int						process_export_argument(char *arg, t_data *data);
char					*extract_key(char *arg, char *eq);
char					*extract_value(char *eq);
int						handle_unset_arguments(char **args, t_data *data);
void					unset_env_variable(t_env **env, char *key);
void					free_env_variable(t_env *node);
t_env					*create_env_node(char *env_entry);
void					add_env_node(t_env **head, t_env *new_node);
int						count_env_variables(t_env *env);
char					**allocate_env_array(int count);
void					convert_env_list_to_array(t_env *env, char **array);
void					setup_pipes_for_child(int index, t_pipe_info *pi);
void					close_unused_pipes(t_pipe_info *pi);
void					execute_builtin_or_command(t_command *cmd,
							t_data *data);
int						create_child_process(t_command *cmd, int i,
							t_pipe_info *pi, t_data *data);
void					close_pipeline_pipes(t_pipe_info *pi);
int						wait_for_children(int n);
char					*get_path_env(void);
char					*build_executable_path(const char *directory,
							const char *cmd);
char					*search_executable_in_paths(char *paths,
							const char *cmd);
int						save_stdin(void);
char					*get_executable_path(t_command *cmd);
int						fork_and_execute(char *exec_path, t_command *cmd,
							t_data *data);
int						execute_external_command(t_command *cmd, t_data *data);
t_token					*allocate_token(void);
char					*duplicate_token_value(const char *str);
t_token_type			determine_token_type(const char *str);
void					initialize_token(t_token *token, const char *str);
void					handle_whitespace(t_token_state *state);
void					handle_quotes(t_token_state *state);
void					handle_escape_character(t_token_state *state);
void					handle_dollar_sign(t_token_state *state,
							int exit_status, t_env *env);
void					process_regular_character(t_token_state *state);
t_token_state			initialize_token_state(const char *input);
void					cleanup_token_state(t_token_state *state);
void					append_token_to_list(t_token **head, t_token **tail,
							t_token *new_token);
t_token					*process_tokens(t_token_state *state, t_env *env);
void					restore_stdin(int saved);
int						save_stdin(void);
#endif
