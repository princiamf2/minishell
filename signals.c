/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 20:58:02 by michel            #+#    #+#             */
/*   Updated: 2025/05/19 19:50:51 by mm-furi          ###   ########.fr       */
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

void restore_default_signals(void)
{
	rl_catch_signals = 1;
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, handle_sigquit);
}

void sigint_heredoc_handler(int signo)
{
    (void)signo;
	g_exit_status = SIGINT;
    rl_replace_line("", 0);
	rl_done = 1;
}
