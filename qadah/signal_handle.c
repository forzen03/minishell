/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/02/03 17:44:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_int_handle(int sig)
{
	g_signal = sig;//so i can know in the readline loop that ctl+c is pressed and change the exit status to 130 and retore g_sigal to 0
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_interactive_signals(void)
{
	signal(SIGINT, signal_int_handle);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_execution_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	restore_default_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	exit_handle(t_term *term, int exit_status)
{
	rl_clear_history();
	tcsetattr(STDIN_FILENO, TCSANOW, &term->original_term);
	write(2, "exit\n", 6);
	exit(exit_status);
}
