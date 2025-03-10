/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 13:31:57 by nicolsan          #+#    #+#             */
/*   Updated: 2025/03/10 16:13:17 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/wait.h>


t_env *g_env = NULL;
int g_exit_status = 0;

static t_cmdlist *parse_line(t_token *tokens);
static t_andor *parse_andor(t_token **cur) __attribute__((unused));
static t_command *parse_pipeline(t_token **cur) __attribute__((unused));
static t_command *parse_command(t_token **cur) __attribute__((unused));
static t_command *parse_subshell(t_token **cur) __attribute__((unused));
static int execute_pipeline(t_command *pipeline, t_env *env) __attribute__((unused));
static int execute_andor(t_andor *list, t_env *env) __attribute__((unused));
static void free_cmdlist(t_cmdlist *list);
static char *read_heredoc(const char *delim, bool quoted) __attribute__((unused));
static bool validate_tokens(t_token *tokens) __attribute__((unused));
static char **glob_pattern(const char *pattern) __attribute__((unused));
static void debug_print_redirs(t_redir *redir) __attribute__((unused));
static bool validate_tokens_adv(t_token *tokens) __attribute__((unused));
static char *find_command_path(const char *cmd, t_env *env) __attribute__((unused));
static int execute_single_command(t_command *cmd, t_env *env) __attribute__((unused));


static char *find_command_path(const char *cmd, t_env *env) {
    (void)env;
    if (access(cmd, X_OK) == 0)
        return strdup(cmd);
    return NULL;
}

static bool validate_tokens(t_token *tokens)
{
	int paren_balance = 0;
	t_token *cur = tokens;
	bool prev_was_operator = false;
	while (cur)
	{
		if (strcmp(cur->value, "(") == 0)
			paren_balance++;
		else if (strcmp(cur->value, ")") == 0)
		{
			paren_balance--;
			if (paren_balance < 0)
			{
				fprintf(stderr, "[erreur] Parenthèses desequilibrees : trop de ')'\n");
				return (false);
			}
		}
		if (cur->type == REDIR_IN || cur->type == REDIR_OUT ||
			cur->type == REDIR_APPEND || cur->type == HEREDOC)
			{
			if (!cur->next || cur->next->type != WORD)
			{
				fprintf(stderr, "[erreur] Redirection sans cible : \"%s\"\n", cur->value);
				return (false);
			}
		}
		if (cur->type == AND_IF || cur->type == OR_IF)
		{
			if (prev_was_operator)
			{
				fprintf(stderr, "[erreur] Deux operateurs logiques consecutifs\n");
				return (false);
			}
			prev_was_operator = true;
		}
		else if (cur->type != SEMICOLON)
		{
			prev_was_operator = false;
		}
		cur = cur->next;
	}
	if (paren_balance != 0)
	{
		fprintf(stderr, "[erreur] Parenthèses desequilibrees : balance = %d\n", paren_balance);
		return (false);
	}
	return (true);
}

static bool validate_tokens_adv(t_token *tokens)
{
	if (!tokens)
		return (false);

	if (tokens->type == AND_IF || tokens->type == OR_IF || tokens->type == PIPE)
	{
		fprintf(stderr, "[erreur] Syntax erreur: la commande ne peut commencer par un operateur\n");
		return (false);
	}

	t_token *prev = tokens;
	t_token *cur = tokens->next;
	while (cur)
	{
		if ((cur->type == AND_IF || cur->type == OR_IF || cur->type == PIPE) &&
			(prev->type == AND_IF || prev->type == OR_IF || prev->type == PIPE))
		{
			fprintf(stderr, "[erreur] Syntax erreur: operateurs consecutifs detectes\n");
			return (false);
		}
		if (cur->type == REDIR_IN || cur->type == REDIR_OUT ||
			cur->type == REDIR_APPEND || cur->type == HEREDOC)
		{
			if (!cur->next || cur->next->type != WORD)
			{
				fprintf(stderr, "[erreur] Syntax erreur: redirection \"%s\" sans cible\n", cur->value);
				return (false);
			}
		}
		prev = cur;
		cur = cur->next;
	}

	if (prev->type == AND_IF || prev->type == OR_IF || prev->type == PIPE)
	{
		fprintf(stderr, "[erreur] Syntax error: la commande ne peut se terminer par un operateur\n");
		return (false);
	}

	return (true);
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

t_env *env_init(char **envp)
{
	(void)envp;
	return NULL;
}

void free_env(t_env *env)
{
	(void)env;
}

static int execute_single_command(t_command *cmd, t_env *env)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {  /* Enfant */
        /* Ici, vous devriez appliquer les redirections de cmd->redirs,
           par exemple en itérant sur la liste et en appelant open/dup2 */
        char *path = find_command_path(cmd->argv[0], env);
        if (!path) {
            fprintf(stderr, "minishell: %s: command not found\n", cmd->argv[0]);
            exit(127);
        }
        char **envp = env_to_array(env);
        execve(path, cmd->argv, envp);
        perror("execve");
        exit(126);
    } else {  /* Parent */
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
}

char *env_get(t_env *env, const char *key)
{
	(void)env;
	(void)key;
	return NULL;
}

void	env_set(t_env **env, const char *key, const char *value)
{
	(void)env;
	(void)key;
	(void)value;
}

void	env_unset(t_env **env, const char *key)
{
	(void)env;
	(void)key;
}

char	**env_to_array(t_env *env)
{
	(void)env;
	return (NULL);
}

int	builtin_echo(char **argv)
{
	(void)argv;
	return (0);
}

int	builtin_cd(char **argv, t_env *env)
{
	(void)argv;
	(void)env;
	return (0);
}

int	builtin_pwd(void)
{
	return (0);
}

int	builtin_export(char **argv, t_env **env)
{
	(void)argv;
	(void)env;
	return (0);
}

int	builtin_unset(char **argv, t_env **env)
{
	(void)argv;
	(void)env;
	return (0);
}

int	builtin_env(t_env *env)
{
	(void)env;
	return (0);
}

int	builtin_exit(char **argv, bool is_child, t_env *env)
{
	(void)argv;
	(void)is_child;
	(void)env;
	exit(0);
	return (0);
}

bool	is_whitespace(char c)
{
	return (c == ' ' || c == '\t');
}

size_t	skip_whitespace(const char *input, size_t i)
{
	while (input[i] && is_whitespace(input[i]))
		i++;
	return (i);
}

static void	append_to_buffer(char *buffer, size_t *index, const char *str)
{
	size_t j = 0;
	while (str[j] && *index < 4096 - 1)
	{
		buffer[*index] = str[j];
		(*index)++;
		j++;
	}
}

static void	handle_dollar_question(const char *input, size_t *i, char *buffer, size_t *index)
{
	(void)input;
	char num[16];
	snprintf(num, sizeof(num), "%d", g_exit_status);
	append_to_buffer(buffer, index, num);
	*i += 2;
}

static void	handle_dollar_variable(const char *input, size_t *i, char *buffer, size_t *index)
{
	size_t start = *i + 1, len = 0;
	while (input[start + len] &&
		   (isalnum(input[start + len]) || input[start + len] == '_'))
		len++;
	if (len)
	{
		char var[256];
		if (len >= sizeof(var))
			len = sizeof(var) - 1;
		strncpy(var, input + start, len);
		var[len] = '\0';
		char *val = env_get(g_env, var);
		if (!val)
			val = "";
		append_to_buffer(buffer, index, val);
		*i = start + len;
	} else
	{
		buffer[(*index)++] = input[(*i)++];
	}
}

static void	process_token_char(const char *input, size_t *i, bool *in_single, bool *in_double, char *buffer, size_t *index)
{
	char c = input[*i];
	if (!(*in_single) && !(*in_double) && is_whitespace(c))
		return;
	if (c == '\'' && !(*in_double))
		{ *in_single = !(*in_single); (*i)++; return; }
	if (c == '\"' && !(*in_single))
		{ *in_double = !(*in_double); (*i)++; return; }
	if (c == '\\' && !(*in_single))
		{ (*i)++; if (!input[*i]) return; c = input[*i]; }
	if (c == '$' && !(*in_single))
	{
		if (input[*i + 1] == '?')
			handle_dollar_question(input, i, buffer, index);
		else
			handle_dollar_variable(input, i, buffer, index);
		return;
	}
	buffer[(*index)++] = c;
	(*i)++;
}

static char *collect_token(const char *input, size_t *i)
{
	char buf[4096];
	size_t idx = 0;
	bool in_single = false, in_double = false;

	if (!in_single && !in_double && (input[*i] == '(' || input[*i] == ')'))
	{
		buf[idx++] = input[*i];
		(*i)++;
		buf[idx] = '\0';
		return strdup(buf);
	}

	while (input[*i])
	{
		if (!in_single && !in_double && is_whitespace(input[*i]))
			break;
		if (!in_single && !in_double && (input[*i] == '(' || input[*i] == ')'))
			break;
		process_token_char(input, i, &in_single, &in_double, buf, &idx);
	}
	buf[idx] = '\0';
	return strdup(buf);
}

static t_token *create_token(const char *str)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
	{
		perror("malloc");
		return NULL;
	}
	token->value = strdup(str);
	if (!token->value)
	{
		free(token);
		return NULL;
	}
	if (strcmp(str, "|") == 0)
		token->type = PIPE;
	else if (strcmp(str, "&&") == 0)
		token->type = AND_IF;
	else if (strcmp(str, "||") == 0)
		token->type = OR_IF;
	else if (strcmp(str, ";") == 0)
		token->type = SEMICOLON;
	else if (strcmp(str, ">") == 0)
		token->type = REDIR_OUT;
	else if (strcmp(str, "<") == 0)
		token->type = REDIR_IN;
	else if (strcmp(str, ">>") == 0)
		token->type = REDIR_APPEND;
	else if (strcmp(str, "<<") == 0)
		token->type = HEREDOC;
	else
		token->type = WORD;
	token->quoted = false;
	token->next = NULL;
	return token;
}

t_token	*lexer(const char *input)
{
	fprintf(stderr, "[debuger] Entree dans lexer() : \"%s\"\n", input);
	t_token *head = NULL, *tail = NULL;
	size_t i = skip_whitespace(input, 0);
	while (input[i])
	{
		char *tok_str = collect_token(input, &i);
		if (!tok_str)
			break;
		t_token *token = create_token(tok_str);
		free(tok_str);
		if (!token)
			break;
		if (!head)
			head = tail = token;
		else
		{
			tail->next = token; tail = token;
		}
		fprintf(stderr, "[debuger] Token cree : \"%s\"\n", token->value);
		i = skip_whitespace(input, i);
	}
	fprintf(stderr, "[debuger] Fin du lexer()\n");
	return (head);
}

void free_tokens(t_token *tokens)
{
	(void)tokens;
}

t_cmdlist *parse_line(t_token *tokens)
{
	fprintf(stderr, "[debuger] Entree dans parse_line()\n");
	if (!tokens)
	{
		fprintf(stderr, "[debuger] Aucun token a parser\n");
		return NULL;
	}
	t_cmdlist *head = NULL, *tail = NULL;
	while (tokens)
	{
		t_cmdlist *node = malloc(sizeof(t_cmdlist));
		if (!node)
		{
			perror("malloc"); break;
		}
		node->andor = parse_andor(&tokens);
		node->next = NULL;
		if (!head)
			head = tail = node;
		else
		{
			tail->next = node;
			tail = node;
		}
		if (tokens && tokens->type == SEMICOLON)
			tokens = tokens->next;
	}
	fprintf(stderr, "[debuger] Fin de parse_line()\n");
	return head;
}

static t_andor *init_andor(t_command *pipeline)
{
	t_andor *andor = malloc(sizeof(t_andor));
	if (!andor)
	{ perror("malloc"); return NULL; }
	andor->pipeline = pipeline;
	andor->op = 0;
	andor->next = NULL;
	return andor;
}

static void append_andor(t_andor *head, t_andor *node)
{
	t_andor *cur = head;
	while (cur->next)
		cur = cur->next;
	cur->next = node;
}

static t_andor *parse_andor(t_token **cur)
{
	fprintf(stderr, "[debuger] Entree dans parse_andor()\n");
	t_command *pipeline = parse_pipeline(cur);
	if (!pipeline)
	{
		fprintf(stderr, "[debuger] Aucun pipeline trouve\n");
		return NULL;
	}
	t_andor *head = init_andor(pipeline);
	while (*cur && ((*cur)->type == AND_IF || (*cur)->type == OR_IF))
	{
		fprintf(stderr, "[debuger] Operateur logique trouve : \"%s\"\n", (*cur)->value);
		t_token_type op = (*cur)->type;
		*cur = (*cur)->next;
		t_command *next_pipeline = parse_pipeline(cur);
		if (!next_pipeline)
		{
			fprintf(stderr, "[debuger] Erreur: aucun pipeline après l'operateur\n");
			break;
		}
		t_andor *node = init_andor(next_pipeline);
		if (!node)
			break;
		node->op = op;
		append_andor(head, node);
	}
	fprintf(stderr, "[debuger] Fin de parse_andor()\n");
	return head;
}

static t_command *parse_pipeline(t_token **cur)
{
	fprintf(stderr, "[debuger] Entree dans parse_pipeline()\n");
	t_command *first = parse_command(cur);
	if (!first)
	{
		fprintf(stderr, "[debuger] Aucune commande trouvee dans le pipeline\n");
		return NULL;
	}
	t_command *current = first;
	while (*cur && (*cur)->type == PIPE)
	{
		fprintf(stderr, "[debuger] Token PIPE trouve\n");
		*cur = (*cur)->next;
		t_command *next = parse_command(cur);
		if (!next)
		{
			fprintf(stderr, "[debuger] Erreur: commande après PIPE manquante\n");
			break;
		}
		current->next_pipe = next;
		current = next;
	}
	fprintf(stderr, "[debuger] Fin de parse_pipeline()\n");
	return first;
}

static void debug_print_redirs(t_redir *redir)
{
	while (redir)
	{
		fprintf(stderr, "[debuger] Redirection: type %d, target \"%s\"\n",
				redir->type, redir->target);
		redir = redir->next;
	}
}

static bool is_redirection(t_token *token)
{
	return (token->type == REDIR_IN || token->type == REDIR_OUT ||
			token->type == REDIR_APPEND || token->type == HEREDOC);
}

static void parse_redirection(t_command *cmd, t_token **cur)
{
	t_redir *redir = malloc(sizeof(t_redir));
	if (!redir)
	{ perror("malloc"); return; }
	redir->type = (*cur)->type;
	redir->next = cmd->redirs;
	cmd->redirs = redir;
	*cur = (*cur)->next;
	if (!*cur || (*cur)->type != WORD)
	{
		fprintf(stderr, "[debuger] Erreur: redirection sans cible\n");
		free(redir);
		return;
	}
	if (redir->type == HEREDOC)
		redir->target = read_heredoc((*cur)->value, (*cur)->quoted);
	else
		redir->target = strdup((*cur)->value);
	redir->delim_quoted = (*cur)->quoted;
	*cur = (*cur)->next;
}

static void add_argument_to_cmd(t_command *cmd, const char *arg, size_t *argc, size_t *capacity)
{
	if (*argc + 1 >= *capacity)
	{
		*capacity *= 2;
		char **tmp = realloc(cmd->argv, *capacity * sizeof(char *));
		if (!tmp)
		{ perror("realloc"); return; }
		cmd->argv = tmp;
	}
	cmd->argv[(*argc)++] = strdup(arg);
}

static void handle_word_token(t_token **cur, t_command *cmd,
							  size_t *argc, size_t *capacity)
{
	if (strchr((*cur)->value, '*') && !(*cur)->quoted)
	{
		fprintf(stderr, "[debuger] Wildcard detecte dans : \"%s\"\n", (*cur)->value);
		char **matches = glob_pattern((*cur)->value);
		if (matches)
		{
			size_t mi = 0;
			while (matches[mi])
			{
				fprintf(stderr, "[debuger] Correspondance glob : \"%s\"\n", matches[mi]);
				add_argument_to_cmd(cmd, matches[mi], argc, capacity);
				mi++;
			}
			mi = 0;
			while (matches[mi])
			{
				free(matches[mi]);
				mi++;
			}
			free(matches);
			*cur = (*cur)->next;
			return;
		}
	}
	fprintf(stderr, "[debuger] Ajout d'argument : \"%s\"\n", (*cur)->value);
	add_argument_to_cmd(cmd, (*cur)->value, argc, capacity);
	*cur = (*cur)->next;
}


static t_command *init_command(void)
{
	t_command *cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc");
		return NULL;
	}
	cmd->subshell = false;
	cmd->subshell_ast = NULL;
	cmd->redirs = NULL;
	cmd->next_pipe = NULL;
	return cmd;
}

static int parse_command_arguments(t_command *cmd, t_token **cur)
{
	size_t capacity = 8, argc = 0;
	cmd->argv = malloc(capacity * sizeof(char *));
	if (!cmd->argv)
	{
		free(cmd);
		return -1;
	}
	while (*cur && (((*cur)->type == WORD) || is_redirection(*cur)))
	{
		if ((*cur)->type == WORD)
			handle_word_token(cur, cmd, &argc, &capacity);
		else
		{
			fprintf(stderr, "[debuger] Redirection detectee : \"%s\"\n", (*cur)->value);
			parse_redirection(cmd, cur);
		}
	}
	cmd->argv[argc] = NULL;
	return (int)argc;
}

static t_command *parse_command(t_token **cur)
{
	fprintf(stderr, "[debuger] Entree dans parse_command()\n");

	if (*cur && strcmp((*cur)->value, "(") == 0)
	{
		t_command *sub = parse_subshell(cur);
		fprintf(stderr, "[debuger] Fin de parse_command() avec subshell\n");
		return sub;
	}
	t_command *cmd = init_command();
	if (!cmd)
		return NULL;

	int argc = parse_command_arguments(cmd, cur);
	if (argc < 0)
		return NULL;

	fprintf(stderr, "[debuger] Fin de parse_command() avec %d argument(s)\n", argc);
	return cmd;
}

static bool match_pattern(const char *pattern, const char *str)
{
	if (!*pattern)
		return !*str;
	if (*pattern == '*')
	{
		size_t skip = 0;
		while (true)
		{
			if (match_pattern(pattern + 1, str + skip))
				return true;
			if (!str[skip])
				break;
			skip++;
		}
		return false;
	}
	return (*pattern == *str) && match_pattern(pattern + 1, str + 1);
}

static void get_dir_and_pattern(const char *pattern, char *dirpath, size_t size, const char **pat)
{
	const char *slash = strrchr(pattern, '/');
	if (slash)
	{
		size_t len = slash - pattern;
		if (len < size)
		{
			strncpy(dirpath, pattern, len);
			dirpath[len] = '\0';
		} else
		{
			strncpy(dirpath, ".", size);
			dirpath[size - 1] = '\0';
		}
		*pat = slash + 1;
	} else
	{
		strncpy(dirpath, ".", size);
		dirpath[size - 1] = '\0';
		*pat = pattern;
	}
}

static char *build_fullpath(const char *dirpath, const char *filename)
{
	if (strcmp(dirpath, ".") == 0)
		return strdup(filename);
	size_t len_dir = strlen(dirpath);
	size_t len_file = strlen(filename);
	char *fullpath = malloc(len_dir + 1 + len_file + 1);
	if (!fullpath)
		return NULL;
	strcpy(fullpath, dirpath);
	fullpath[len_dir] = '/';
	strcpy(fullpath + len_dir + 1, filename);
	return fullpath;
}

static int add_match(char ***matches, size_t *count, size_t *capacity, const char *dirpath, const char *filename)
{
	if (*count >= *capacity)
	{
		*capacity *= 2;
		char **tmp = realloc(*matches, (*capacity) * sizeof(char *));
		if (!tmp)
			return -1;
		*matches = tmp;
	}
	char *fullpath = build_fullpath(dirpath, filename);
	if (!fullpath)
		return 0;
	(*matches)[(*count)++] = fullpath;
	return 1;
}

static char **read_directory_matches(const char *dirpath, const char *pat)
{
	DIR *dir = opendir(dirpath);
	if (!dir)
		return NULL;
	size_t capacity = 8, count = 0;
	char **matches = malloc(capacity * sizeof(char *));
	if (!matches)
	{
		closedir(dir);
		return NULL;
	}
	struct dirent *entry;
	while ((entry = readdir(dir)) != NULL)
	{
		if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
			continue;
		if (match_pattern(pat, entry->d_name))
		{
			if (add_match(&matches, &count, &capacity, dirpath, entry->d_name) == -1)
				break;
		}
	}
	closedir(dir);
	matches = realloc(matches, (count + 1) * sizeof(char *));
	if (matches)
		matches[count] = NULL;
	return matches;
}

static char **glob_pattern(const char *pattern)
{
	if (!pattern)
		return NULL;
	char dirpath[PATH_MAX];
	const char *pat;
	get_dir_and_pattern(pattern, dirpath, sizeof(dirpath), &pat);
	return read_directory_matches(dirpath, pat);
}

static bool skip_opening_paren(t_token **cur)
{
	if (!cur || !*cur || strcmp((*cur)->value, "(") != 0)
	{
		fprintf(stderr, "[debuger] Erreur: pas de '(' pour subshell\n");
		return false;
	}
	*cur = (*cur)->next;
	return true;
}

static t_token *collect_tokens_until_closing(t_token **cur)
{
	t_token *start = *cur;
	t_token *prev = NULL;
	int balance = 1;
	while (*cur)
	{
		if (strcmp((*cur)->value, "(") == 0)
			balance++;
		else if (strcmp((*cur)->value, ")") == 0)
		{
			balance--;
			if (balance == 0)
				break;
		}
		prev = *cur;
		*cur = (*cur)->next;
	}
	if (!*cur || balance != 0)
	{
		fprintf(stderr, "[debuger] Erreur: parenthèses non equilibrees\n");
		return NULL;
	}
	if (prev)
		prev->next = NULL;
	return start;
}

static t_token *extract_subshell_tokens(t_token **cur)
{
	if (!skip_opening_paren(cur))
		return NULL;
	return collect_tokens_until_closing(cur);
}

static t_cmdlist *build_subshell_ast(t_token *tokens)
{
	return parse_line(tokens);
}

static t_command *create_subshell_command(t_cmdlist *sub_ast)
{
	t_command *cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc");
		return NULL;
	}
	cmd->subshell = true;
	cmd->subshell_ast = sub_ast;
	cmd->argv = NULL;
	cmd->redirs = NULL;
	cmd->next_pipe = NULL;
	return cmd;
}

static t_command *parse_subshell(t_token **cur)
{
	fprintf(stderr, "[debuger] Entree dans parse_subshell()\n");

	t_token *sub_tokens = extract_subshell_tokens(cur);
	if (!sub_tokens)
		return NULL;
	t_cmdlist *sub_ast = build_subshell_ast(sub_tokens);
	*cur = (*cur)->next;
	t_command *cmd = create_subshell_command(sub_ast);
	if (!cmd)
		return NULL;
	fprintf(stderr, "[debuger] Fin de parse_subshell()\n");
	return cmd;
}

static int	execute_pipeline(t_command *pipeline, t_env *env)
{
	(void)pipeline;
	(void)env;
	return 0;
}

static int execute_andor(t_andor *list, t_env *env)
{
	int result = 0;
	bool should_run = true;
	t_andor *node = list;
	while (node)
	{
		if (should_run)
			result = execute_pipeline(node->pipeline, env);
		if (node->op == AND_IF)
			should_run = (result == 0);
		else if (node->op == OR_IF)
			should_run = (result != 0);
		else
			should_run = true;
		node = node->next;
	}
	return result;
}

static void	free_cmdlist(t_cmdlist *list)
{
	(void)list;
}

static void append_line(char **content, size_t *len, size_t *cap, const char *line)
{
	size_t l_len;
	l_len = strlen(line);
	if (*len + l_len + 2 > *cap)
	{
		*cap += l_len + 1024;
		char *tmp = realloc(*content, *cap);
		if (!tmp)
		{
			free(*content);
			exit(1);
		}
		*content = tmp;
	}
	strcpy(*content + *len, line);
	*len += l_len;
	(*content)[(*len)++] = '\n';
	(*content)[*len] = '\0';
}

static char *read_heredoc(const char *delim, bool quoted)
{
	(void)quoted;
	size_t cap = 1024, len = 0;
	char *content = malloc(cap);
	if (!content)
		exit(1);
	content[0] = '\0';

	while (1)
	{
		char *line;
		line = readline("> ");
		if (!line)
		{
			fprintf(stderr, "[debuger] EOF atteint dans heredoc\n");
			break ;
		}
		if (strcmp(line, delim) == 0)
		{
			free(line);
			break ;
		}
		append_line(&content, &len, &cap, line);
		free(line);
	}
	return (content);
}

int	execute_cmdlist(t_cmdlist *commands, t_env *env)
{
	int			last_status;
	t_cmdlist	*node;

	last_status = 0;
	node = commands;
	while (node)
	{
		last_status = execute_andor(node->andor, env);
		node = node->next;
	}
	return (last_status);
}

static void process_input(char *input)
{
	int		 status;
	t_cmdlist   *cmdlist;
	t_token	 *tokens;

	add_history(input);
	tokens = lexer(input);
	free(input);
	if (!tokens)
		return;
	if (!validate_tokens_adv(tokens))
	{
		fprintf(stderr, "[err] Syntaxe invalide.\n");
		free_tokens(tokens);
		return;
	}
	cmdlist = parse_line(tokens);
	free_tokens(tokens);
	if (cmdlist == NULL)
		return;
	status = execute_cmdlist(cmdlist, g_env);
	g_exit_status = status;
	free_cmdlist(cmdlist);
}

void shell_loop(void)
{
	char *input;

	while ((input = readline("minishell$ ")) != NULL)
	{
		if (input[0] != '\0')
			process_input(input);
		else
			free(input);
	}
	fprintf(stderr, "exit\n");
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	g_env = env_init(envp);
	rl_catch_signals = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
	shell_loop();
	free_env(g_env);
	rl_clear_history();
	return (g_exit_status);
}
