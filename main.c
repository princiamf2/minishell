/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:52:32 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/26 19:50:41 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_data	*init_minishell(char **envp)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
	{
		perror("malloc");
		exit(1);
	}
	data->env = env_init(envp);
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
	while ((data->input = readline("minishell$> ")) != NULL)
	{
		if (*data->input)
			add_history(data->input);
		data->tokens = lexer(data->input, data->env);
		free(data->input);
		if (!data->tokens)
			continue ;
		if (!validate_tokens_adv(data->tokens))
		{
			ft_putendl_fd("Syntaxe invalide.\n", 2);
			free_tokens(data->tokens);
			continue ;
		}
		data->cmdlist = parse_line(data->tokens);
		free_tokens(data->tokens);
		if (!data->cmdlist)
			continue ;
		data->exit_status = execute_cmdlist(data->cmdlist, data);
		free_cmdlist(data->cmdlist);
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
