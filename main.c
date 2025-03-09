/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 18:52:32 by mm-furi           #+#    #+#             */
/*   Updated: 2025/03/08 20:42:36 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    char    *line;
    char    **args;
    int     exit_code = 0;
    int     i;

    (void)argc;
    (void)argv;
    while (1)
    {
        line = readline("minishell> ");
        if (!line)
            break;
        if (*line)
            add_history(line);
        args = ft_split(line, ' ');
        if (args && args[0])
            exit_code = execute_command(args, envp);
        free(line);
        i = 0;
        if (args)
        {
            while (args[i])
            {
                free(args[i]);
                i++;
            }
            free(args);
        }
    }
    return (exit_code);
}
