/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_and_more.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:51:37 by nicolsan          #+#    #+#             */
/*   Updated: 2025/05/14 15:55:07 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_external_in_child(t_command *cmd, t_data *data)
{
	char	*exec_path;
	char	**env_array;

	if (ft_strchr(cmd->args[0], '/') != NULL)
		exec_path = ft_strdup(cmd->args[0]);
	else
		exec_path = find_excutable(cmd->args[0], data->env);
	if (!exec_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	env_array = env_to_array(data->env);
	execve(exec_path, cmd->args, env_array);
	perror("minishell: execve");
	free_str_array(env_array);
	free(exec_path);
	exit(126);
}

bool	is_redirection(t_token *token)
{
	if (!token)
		return (false);
	return (token->type == REDIR_IN || token->type == REDIR_OUT
		|| token->type == REDIR_APPEND || token->type == HEREDOC);
}

int	count_commands(t_command *pipeline)
{
	int	count;

	count = 0;
	while (pipeline)
	{
		count++;
		pipeline = pipeline->next_pipe;
	}
	return (count);
}

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc");
		return (NULL);
	}
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->append = 0;
	cmd->heredoc = 0;
	cmd->heredoc_tmp = NULL;
	cmd->subshell = false;
	cmd->redirs = NULL;
	cmd->next_pipe = NULL;
	cmd->subshell_ast = NULL;
	return (cmd);
}

void	free_str_array_child(char **array)
{
	int	i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/*
void	process_assignement(char *input, t_data *data)
{
	char	*eq;

	eq = ft_strchr(input, '=');
	if (!eq)
		return ;
	*eq = '\0';
	env_set(&(data->env), input, eq + 1);
	*eq = '=';
}
	*/

/*
char	*get_first_token(const char *input)
{
	int	i;

	i = 0;
	while (input[i] && input[i] != ' ' && input[i] != '\t')
		i++;
	return (ft_substr(input, 0, i));
}
	*/

/*
void	skip_semicolon(t_token **tokens)
{
	if (*tokens && (*tokens)->type == SEMICOLON)
		*tokens = (*tokens)->next;
}
*/

/*
int	save_stdin(void)
{
	int	fd;

	fd = dup(STDIN_FILENO);
	if (fd < 0)
		perror("dup");
	return (fd);
}
*/

/*
void	print_env_array(char **env_array)
{
	int	i;

	i = 0;
	if (!env_array)
	{
		printf("env_array est NULL\n");
		return ;
	}
	while (env_array[i])
	{
		printf("%s\n", env_array[i]);
		i++;
	}
}
*/
/*
t_redir	*find_heredoc_redir(t_redir *redirs)
{
	while (redirs)
	{
		if (redirs->type == HEREDOC)
		{
			return (redirs);
		}
		redirs = redirs->next;
	}
	return (NULL);
}
*/

/*
int	find_env_index(char **env, char *var)
{
	int		i;
	size_t	key_len;
	char	*eq;

	i = 0;
	eq = ft_strchr(var, '=');
	if (!eq)
		return (-1);
	key_len = eq - var;
	while (env[i])
	{
		if (!ft_strncmp(env[i], var, key_len) && env[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}
*/

/*
char	**add_env_variable(char **env, char *new_var)
{
	int		cnt;
	int		i;
	char	**new_env;

	cnt = 0;
	while (env[cnt])
		cnt++;
	new_env = malloc((cnt + 2) * sizeof(char *));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < cnt)
	{
		new_env[i] = env[i];
		i++;
	}
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;
	free(env);
	return (new_env);
}
*/

/*
int	find_env_index_unset(char **env, char *var)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(var);
	while (env[i])
	{
		if (!ft_strncmp(env[i], var, len) && env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}
*/
