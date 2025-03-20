/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:52:32 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/20 20:01:33 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    t_data *data;

    (void)argc;
    (void)argv;
    data = malloc(sizeof(t_data));
	if (!data)
	{
		perror("malloc");
		return (1);
	}
    data->env = env_init(envp);
    data->exit_status = 0;
    data->input = NULL;
    data->tokens = NULL;
    data->cmdlist = NULL;
    rl_catch_signals = 0;
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);
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
    ft_putendl_fd("exit", 2);
    rl_clear_history();
    free_env(data->env);
    return (data->exit_status);
}
