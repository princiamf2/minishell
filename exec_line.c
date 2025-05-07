/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: michel <michel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 21:59:24 by michel            #+#    #+#             */
/*   Updated: 2025/05/07 22:00:03 by michel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void exec_line(const char *cmd, t_data *data)
{
    t_token   *tokens;
    t_cmdlist *cmdlist;

    tokens = lexer(cmd, data->env, data);
    if (!tokens)
        return ;
    cmdlist = parse_line(tokens);
    free_tokens(tokens);
    if (!cmdlist)
        return ;
    data->exit_status = execute_cmdlist(cmdlist, data);
    free_cmdlist(cmdlist);
}