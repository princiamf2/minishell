/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:52:32 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/29 14:52:31 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "global_vars.h"
#include "minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

t_data	*init_minishell(char **envp)
{
	t_data	*data;
	char	cwd[PATH_MAX];

	data = malloc(sizeof(t_data));
	if (!data)
	{
		perror("malloc");
		exit(1);
	}
	data->env = env_init(envp);
	if (!data->env)
	{
		if (getcwd(cwd, sizeof(cwd)))
			env_set(&(data->env), "PWD", cwd);
		env_set(&(data->env), "SHLVL", "1");
		env_set(&(data->env), "_", "./minishell");
		env_set(&(data->env), "PATH", "/usr/bin:/bin");
	}
	data->local_vars = NULL;
	data->exit_status = 0;
	data->input = NULL;
	data->tokens = NULL;
	data->cmdlist = NULL;
	return (data);
}

void	process_input(t_data *data)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$> ");
		if (line == NULL && g_exit_status == 130)
		{
			g_exit_status = 0;
			continue ;
		}
		if (line == NULL)
			break ;
		if (line[0] == '\0')
		{
			free(line);
			continue ;
		}
		add_history(line);
		data->input = line;
		process_line(data);
	}
}

int	clean_minishell(t_data *data)
{
	int	exit_status;

	ft_putendl_fd("exit", 2);
	rl_clear_history();
	free_env(data->env);
	exit_status = data->exit_status;
	free(data);
	return (exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	*data;
	int		exit_status;

	(void)argc;
	(void)argv;
	data = init_minishell(envp);
	set_minishell_signals();
	process_input(data);
	exit_status = clean_minishell(data);
	return (exit_status);
}
