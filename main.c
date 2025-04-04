/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:52:32 by mm-furi           #+#    #+#             */
/*   Updated: 2025/04/04 13:59:47 by mm-furi          ###   ########.fr       */
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
	data->exit_status = 0;
	data->input = NULL;
	data->tokens = NULL;
	data->cmdlist = NULL;
	return (data);
}

void	set_minishell_signals(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

void	process_input(t_data *data)
{
	data->input = readline("minishell$> ");
	while (data->input != NULL)
	{
		process_line(data);
		data->input = readline("minishell$> ");
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
