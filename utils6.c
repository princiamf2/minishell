/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 15:34:47 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/04 15:48:36 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	skip_opening_paren(t_token **cur)
{
	if (!cur || !*cur || ft_strcmp((*cur)->value, "(") != 0)
		return (false);
	*cur = (*cur)->next;
	return (true);
}

t_token	*collect_tokens_until_closing(t_token **cur)
{
	t_token	*start;
	t_token	*prev;
	int		balance;

	start = *cur;
	prev = NULL;
	balance = 1;
	while (*cur)
	{
		if (ft_strcmp((*cur)->value, "(") == 0)
			balance++;
		else if (ft_strcmp((*cur)->value, ")") == 0)
		{
			balance--;
			if (balance == 0)
				break ;
		}
		prev = *cur;
		*cur = (*cur)->next;
	}
	if (!*cur || balance != 0)
	{
		ft_putendl_fd("Erreur de syntaxe : parenthèse fermante manquante", 2);
        free_tokens(start);
		return (NULL);
	}
	if (prev)
		prev->next = NULL;
	return (start);
}

char	**glob_pattern(const char *pattern)
{
	char		dirpath[PATH_MAX];
	const char	*pat;

	if (!pattern)
		return (NULL);
	get_dir_and_pattern(pattern, dirpath, sizeof(dirpath), &pat);
	return (read_directory_matches(dirpath, pat));
}

int	fork_and_execute(char *exec_path, t_command *cmd, t_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		free(exec_path);
		return (1);
	}
	if (pid == 0)
	{
		if (handle_redirection(cmd) < 0)
			exit(1);
		if (execve(exec_path, cmd->args, env_to_array(data->env)) == -1)
		{
			perror("execve");
			exit(126);
		}
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

void	skip_semicolon(t_token **tokens)
{
	if (*tokens && (*tokens)->type == SEMICOLON)
		*tokens = (*tokens)->next;
}
