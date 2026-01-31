/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 18:23:05 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_int_handle(int sig)
{
    (void)sig;
    g_exit_status = 130;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void	signal_handle(void)
{
    signal(SIGINT, signal_int_handle);
    signal(SIGQUIT, SIG_IGN);
}

void	exit_handle(t_term *term)
{
    rl_clear_history();
    tcsetattr(STDIN_FILENO, TCSANOW, &term->original_term);
    write(2, "exit\n", 6);
    exit(g_exit_status);
}

int	not_all_space(char *line)
{
    int	i;

    i = 0;
    while (line[i])
    {
        if (line[i] != ' ' && line[i] != '\t')
            return (1);
        i++;
    }
    return (0);
}