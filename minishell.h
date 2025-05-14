/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 14:30:47 by mm-furi           #+#    #+#             */
/*   Updated: 2025/05/14 20:19:45 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "global_vars.h"
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
	char				*heredoc_tmp;
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

typedef struct s_gloinfo
{
	DIR					*dir;
	char				**matches;
	size_t				count;
	size_t				capacity;
	char				*dirpath;
}						t_globinfo;

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
	t_env				*local_vars;
	t_cmdlist			*cmdlist;
	t_token				*tokens;
	t_pipe_info			*pipe;
	t_command			*cmd;
	char				*input;
	int					exit_status;
}						t_data;

typedef struct s_interp_params
{
	char				quote_char;
	size_t				start;
	size_t				end;
	size_t				len;
}						t_interp_params;

typedef struct s_interp_ctx
{
	const char			*raw;
	char				*res;
	size_t				*i_ptr;
	size_t				*res_idx_ptr;
	t_interp_params		params;
}						t_interp_ctx;

typedef struct s_token_list
{
	t_token				*head;
	t_token				*tail;
}						t_token_list;

typedef struct s_export_kv
{
	char				*key;
	char				*raw_value;
	char				*eq;
	bool				append;
}						t_export_kv;

char					*ft_strtok(char *s, const char *delim);
char					*find_excutable(const char *cmd, t_env *env);
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
t_token					*create_token(const char *str);
bool					is_whitespace(char c);
size_t					skip_whitespace(const char *input, size_t i);

void					handle_dollar_question(const char *input, size_t *i,
							t_buffer *buf, t_data *data);
void					handle_dollar_variable(const char *input, size_t *i,
							t_buffer *buf, t_data *data);
char					*collect_token(t_token_state *state, t_data *data);
void					process_token_char(t_token_state *state,
							int exit_status, t_data *data);
t_token					*lexer(const char *input, t_env *env, t_data *data);
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
int						add_match(t_globinfo *info, const char *dirpath,
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
int						is_valid_identifier(const char *str);
int						create_child_process(t_command *cmd, int i,
							t_pipe_info *pi, t_data *data);
void					close_pipeline_pipes(t_pipe_info *pi);
int						wait_for_children(int n);
char					*get_path_env(t_env *env);
char					*build_executable_path(const char *directory,
							const char *cmd);
char					*search_executable_in_paths(char *paths,
							const char *cmd);
int						save_stdin(void);
char					*get_executable_path(t_command *cmd, t_data *data);
int						fork_and_execute(char *exec_path, t_command *cmd,
							t_data *data);
int						fork_and_execute_no_redir(char *exec_path,
							t_command *cmd, t_data *data);
int						execute_external_command(t_command *cmd, t_data *data);
t_token					*allocate_token(void);
char					*duplicate_token_value(const char *str);
t_token_type			determine_token_type(const char *str);
void					initialize_token(t_token *token, const char *str);
void					handle_whitespace(t_token_state *state);
void					handle_quotes(t_token_state *state);
void					handle_escape_character(t_token_state *state);
void					handle_dollar_sign(t_token_state *state,
							int exit_status_UNUSED, t_data *data);
void					process_regular_character(t_token_state *state);
t_token_state			initialize_token_state(const char *input);
void					cleanup_token_state(t_token_state *state);
void					append_token_to_list(t_token **head, t_token **tail,
							t_token *new_token);
t_token					*process_tokens(t_token_state *state, t_data *data);
void					restore_stdin(int saved);
int						save_stdin(void);
t_env					*find_env_node(t_env *head, const char *key);
void					update_node_value(t_env *node, const char *val);
t_env					*create_env_node2(const char *key, const char *val);
int						execute_child(t_command *cmd, int index,
							t_pipe_info *pi, t_data *data);
int						*create_pipes(int n);
void					skip_semicolon(t_token **tokens);
bool					update_paren(t_token *cur, int *paren_balance);
bool					validate_redirection(t_token *cur);
bool					update_operator(t_token *cur, bool *prev_operator);
bool					is_operator(t_token *token);
bool					adjacent_operators(t_token *prev, t_token *cur);
void					add_all_matches_to_cmd(char **matches, t_command *cmd,
							size_t *argc, size_t *capacity);
void					free_matches(char **matches);
bool					try_expand_glob(t_token **cur, t_command *cmd,
							size_t *argc, size_t *capacity);
bool					process_heredoc_line(const char *delim, char **content,
							size_t *len, size_t *cap);
bool					is_valid_entry(struct dirent *entry);
int						process_directory_entries(t_globinfo *info,
							const char *dirpath, const char *pat);
char					**finalize_matches(char **matches, size_t count);
bool					handle_in_redirect(t_token_state *state);
bool					handle_out_redirect(t_token_state *state);
bool					handle_pipe_operator(t_token_state *state);
bool					handle_amp_operator(t_token_state *state);
bool					handle_special_operators(t_token_state *state);
bool					handle_semicolon_operator(t_token_state *state);
void					free_command_args(t_command *cmd);
void					free_command_redirs(t_command *cmd);
void					free_pipeline(t_command *cmd);
void					*ft_realloc(void *ptr, size_t old_size,
							size_t new_size);
void					process_assignement(char *input, t_data *data);
void					expand_token(t_token *token, t_env *env,
							t_env *local_vars);
void					expand_tokens(t_token *tokens, t_env *env,
							t_env *local_vars);
int						handle_no_command_arguments(t_command *cmd,
							int saved_stdin, int saved_stdout);
void					process_line(t_data *data);
int						init_cmd_args(t_command *cmd, size_t *capacity,
							size_t *argc);
void					collect_cmd_args(t_command *cmd, t_token **cur,
							size_t *argc, size_t *capacity);
int						builtin_exit(char **args, t_data *data);
int						is_option_n(const char *str);
char					*get_first_token(const char *input);
void					local_set(t_env **local, const char *key,
							const char *val);
void					process_local_assignmment(char *input, t_data *data);
int						execute_subshell(t_command *cmd, t_data *data);
t_command				*parse_subshell_command(t_token **cur);
t_command				*parse_regular_command(t_token **cur);
t_cmdlist				*parse_subshell_tokens(t_token *sub_tokens_head);
t_token					*collect_and_validate_subshell_tokens(t_token **cur);
void					child_process_subshell(t_command *cmd, t_data *data);
int						parent_process_subshell(pid_t pid);
char					*check_initial_paren(t_token_state *state);
bool					should_break_collecting(t_token_state *state);
char					*finalize_collected_token(t_token_state *state);
bool					handle_special_input_cases(t_data *data);
bool					lex_and_validate_input(t_data *data);
void					parse_execute_and_cleanup(t_data *data);
bool					ensure_buffer_capacity(t_buffer *buf,
							size_t additional_len);
bool					ensure_args_capacity(t_command *cmd, size_t argc,
							size_t *capacity);
t_token					*duplicate_original_token(t_token *original);
bool					should_break_subshell_loop(t_token *token,
							int *balance);
t_token					*finalize_subshell_collection(t_token *head,
							t_token **cur, t_token *current_original,
							int balance);
t_token					*process_subshell_tokens_loop(t_token **cur,
							int *balance_ptr, t_token **head_ptr,
							t_token **tail_ptr);
void					ensure_content_capacity(char **content_ptr,
							size_t current_len, size_t *cap_ptr,
							size_t line_len);
size_t					get_digit_count(int n);
void					fill_str(char *str, long num, size_t len);
int						is_in_set(char c, const char *set);
size_t					get_actual_length(char const *s, unsigned int start,
							size_t len);
char					*interpret_raw_value(const char *raw_value);

int						compare_env_vars(const void *a, const void *b);
void					free_env_array(char **env_array);

int						handle_export_no_args(t_data *data);
void					print_export_key_value(const char *env_var,
							const char *equal_sign);
void					print_export_formatted(const char *env_var);
void					print_export_key_only(const char *env_var);

bool					save_original_fds(int *saved_stdin_ptr,
							int *saved_stdout_ptr);

void					get_interp_params(const char *raw_value,
							t_interp_params *params);

void					restore_original_fds(int saved_stdin, int saved_stdout);

void					process_char_for_interpretation(t_interp_ctx *ctx);

size_t					interpret_and_copy(char *result, const char *raw_value,
							t_interp_params params);
bool					handle_redir_output(t_command *cmd,
							const char *target_val, t_token_type type);

bool					handle_redir_input(t_command *cmd,
							const char *target_val);
bool					handle_redir_heredoc(t_command *cmd,
							const char *target_val);
bool					parse_andor_append(t_andor **tail_ptr, t_token **cur);

int						handle_exec_path_not_found(t_command *cmd,
							int saved_stdin, int saved_stdout);

int						dispatch_execution(t_command *cmd, t_data *data);
void					execute_external_in_child(t_command *cmd, t_data *data);

void					free_str_array(char **array);

int						calculate_exit_code(char **args, int current_status);

void					qualify_token_quoting(t_token *token,
							t_token_state *state, size_t start_idx);
bool					handle_empty_raw_token(char *raw_value,
							t_token_state *state, size_t *i_ptr);

bool					process_token_loop_step(t_token_list *list,
							t_token_state *state, t_data *data, size_t *i_ptr);

bool					extract_export_kv(char *arg, t_export_kv *kv);
bool					validate_export_key(const char *key,
							char *arg_for_error);
int						update_or_add_env_var(t_data *data, char *key,
							char *final_value);
int						process_and_update_env(t_data *data, t_export_kv *kv);

bool					process_heredoc_input_line(int fd,
							const char *interpreted_delimiter);

bool					validate_tok_aft_andor(t_token **current_token_ptr_ptr);

bool					parse_and_append_cmdlist_node(t_cmdlist **head_ptr,
							t_cmdlist **tail_ptr,
							t_token **current_token_ptr_ptr);
bool					populate_and_finalize_command(t_command *cmd,
							t_token **cur, size_t *argc_ptr,
							size_t *capacity_ptr);

t_command				*init_regular_command_struct(size_t *argc_ptr,
							size_t *capacity_ptr);
bool					ensure_globinfo_capacity(t_globinfo *info);
void					free_str_array_child(char **array);
void					child_process_exec(char *exec_path, t_command *cmd,
							t_data *data);
int						parent_process_wait(pid_t pid);
void					child_process_exec_no_redir(char *exec_path,
							t_command *cmd, t_data *data);
char					*find_variable_value(const char *var_name,
							t_data *data);
size_t					extract_variable_name(const char *input, size_t start,
							char *var_buffer, size_t buffer_size);
bool					link_pipeline_commands(t_command **current_ptr,
							t_token **cur, t_command *first_for_cleanup);
bool					extract_export_kv(char *arg, t_export_kv *kv);
bool					validate_export_key(const char *key,
							char *arg_for_error);

int						update_or_add_env_var(t_data *data, char *key,
							char *final_value);

int						process_and_update_env(t_data *data, t_export_kv *kv);
char					*format_env_node_to_string(t_env *node);
size_t					get_digit_count(int n);

void					fill_str(char *str, long num, size_t len);
int						is_in_set(char c, const char *set);
int						is_numeric(const char *str);
size_t					get_actual_length(char const *s, unsigned int start,
							size_t len);
void					set_minishell_signals(void);
void					append_char_as_string(t_buffer *buffer, char c);
bool					handle_param_expansion(t_token_state *state,
							t_buffer *buf, t_data *data);
bool					handle_arith_expansion(t_token_state *state,
							t_buffer *buf, t_data *data);
bool					handle_dollar_subst(t_token_state *state, t_buffer *buf,
							t_data *data);
bool					handle_backtick_substitution(t_token_state *state,
							t_buffer *buf, t_data *data);
char					*expand_param(const char *expr, t_data *data);
long					eval_arith(const char *s);
char					*run_command_substitution(const char *cmd,
							t_data *data);
char					*expand_vars_in_str(const char *s, t_env *env);
void					exec_line(const char *cmd, t_data *data);
void					do_export_no_assign(t_data *data, t_export_kv *kv);
int						do_export_append(t_data *data, t_export_kv *kv);
int						do_export_assign(t_data *data, t_export_kv *kv);
char					*read_complete_line(void);
char					*append_char(char *res, char c);
char					*append_var(char *res, const char *s, t_env *env,
							int *i);
void					handle_append_flag(char *arg, t_export_kv *kv);
void					handle_equal_sign(char *arg, t_export_kv *kv);
char					*extract_arith_expr(t_token_state *st);
char					*extract_subst_cmd(t_token_state *st);
char					*read_subst_output(const char *cmd, t_data *data);
char					*get_env_value(const char *var, t_data *data);
char					*apply_param_op(const char *var, const char *op,
							const char *word, t_data *data);
int						prepare_all_heredocs(t_cmdlist *cmdlist);
int						process_one_heredoc(t_command *cmd);
int						process_pipeline_heredocs(t_command *cmd);
void					increment_shlvl(char ***envp);
char					**copy_env(char **envp);
char					**append_env(char **envp, char *newvar);
#endif
