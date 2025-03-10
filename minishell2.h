/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 13:31:46 by nicolsan          #+#    #+#             */
/*   Updated: 2025/03/06 15:18:37 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdbool.h>
# include <stddef.h>
# include <limits.h>

/* ============================= */
/*         TYPES ET STRUCTS      */
/* ============================= */

/* Token types */
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
}	t_token_type;

/* Structure pour un token */
typedef struct s_token
{
	char				*value;
	t_token_type		type;
	bool				quoted;
	struct s_token		*next;
}						t_token;

/* Structure pour une variable d'environnement */
typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

/* Structure pour une commande */
typedef struct s_command
{
	bool				subshell;
	char				**argv;
	struct s_redir		*redirs;
	struct s_command	*next_pipe;
	struct s_cmdlist	*subshell_ast;
}							t_command;

/* Structure pour une redirection */
typedef struct s_redir
{
	t_token_type	type;
	char			*target;
	bool			delim_quoted;
	struct s_redir	*next;
}			t_redir;

/* Structure pour une chaîne and/or */
typedef struct s_andor
{
	t_command		*pipeline;
	t_token_type	op;
	struct s_andor	*next;
}			t_andor;

/* Structure pour une liste de commandes (séparées par ';') */
typedef struct s_cmdlist
{
	t_andor				*andor;
	struct s_cmdlist	*next;
}			t_cmdlist;

/* ============================= */
/*    DÉCLARATIONS PUBLIQUES     */
/* ============================= */

/* Gestion de l'environnement */
t_env	*env_init(char **envp);
void	free_env(t_env *env);
char	*env_get(t_env *env, const char *key);
void	env_set(t_env **env, const char *key, const char *value);
void	env_unset(t_env **env, const char *key);
char	**env_to_array(t_env *env);

/* Builtins */
int		builtin_echo(char **argv);
int		builtin_cd(char **argv, t_env *env);
int		builtin_pwd(void);
int		builtin_export(char **argv, t_env **env);
int		builtin_unset(char **argv, t_env **env);
int		builtin_env(t_env *env);
int		builtin_exit(char **argv, bool is_child, t_env *env);

/* Exécution */
int		execute_cmdlist(t_cmdlist *commands, t_env *env);

/* Lexer */
t_token	*lexer(const char *input);
void	free_tokens(t_token *tokens);

#endif /* MINISHELL_H */
