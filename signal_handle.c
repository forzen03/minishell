/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 17:24:28 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	signal_int_handle()
{
	g_exit_status = 130;
	write(1, "\n", 1);
	rl_on_new_line();//for putting the cursor on new line
	rl_replace_line("", 0);//replace it with empty line and dont clear the histroy for
	rl_redisplay();//rewrite the minishell$ prompt
}

void	signal_handle()
{
	signal(SIGINT, signal_int_handle);//ctr+c
	signal(SIGQUIT, SIG_IGN);//ctl+\/\/
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
