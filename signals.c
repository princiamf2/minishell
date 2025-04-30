/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicolsan <nicolsan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:58:02 by michel            #+#    #+#             */
/*   Updated: 2025/04/30 13:49:29 by nicolsan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_minishell_signals(void)
{
	rl_catch_signals = 0;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

void	handle_sigint(int sig)
{
	(void)sig;
	g_exit_status = 130;
	rl_replace_line("", 0);
	rl_done = 1;
	write(STDOUT_FILENO, "\n", 1);
}

void	handle_sigquit(int sig)
{
	(void)sig;
}
