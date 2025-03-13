/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils11.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 22:24:42 by michel            #+#    #+#             */
/*   Updated: 2025/03/12 23:50:22 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_command(t_command *cmd)
{
    int i;
    t_redir *redir;
    t_command *next_pipe;

    if (!cmd)
        return;
    // Libérer le tableau d'arguments
    if (cmd->args)
    {
        i = 0;
        while (cmd->args[i])
        {
            free(cmd->args[i]);
            i++;
        }
        free(cmd->args);
    }
    // Libérer la liste des redirections
    while (cmd->redirs)
    {
        redir = cmd->redirs;
        cmd->redirs = cmd->redirs->next;
        if (redir->target)
            free(redir->target);
        free(redir);
    }
    // Libérer les commandes en pipe (s'il y en a)
    next_pipe = cmd->next_pipe;
    free(cmd);
    free_command(next_pipe);
}

int count_commands(t_command *pipeline)
{
    int count = 0;
    while (pipeline)
    {
        count++;
        pipeline = pipeline->next_pipe;
    }
    return (count);
}
