/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 17:24:28 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_signal = 0;

void    print_banner(void)
{
    printf("\n");
    printf(RED "███████╗" BLACK "███████╗" BLACK "████████╗" WHITE " █████╗ " WHITE "███████╗" GREEN " ██████╗ " GREEN "██╗  ██╗\n" RESET);
    printf(RED "██╔════╝" BLACK "██╔════╝" BLACK "╚══██╔══╝" WHITE "██╔══██╗" WHITE "██╔════╝" GREEN "██╔═══██╗" GREEN "╚██╗██╔╝\n" RESET);
    printf(RED "█████╗  " BLACK "███████╗" BLACK "   ██║   " WHITE "███████║" WHITE "█████╗  " GREEN "██║   ██║" GREEN " ╚███╔╝ \n" RESET);
    printf(RED "██╔══╝  " BLACK "╚════██║" BLACK "   ██║   " WHITE "██╔══██║" WHITE "██╔══╝  " GREEN "██║   ██║" GREEN " ██╔██╗ \n" RESET);
    printf(RED "███████╗" BLACK "███████║" BLACK "   ██║   " WHITE "██║  ██║" WHITE "██║     " GREEN "╚██████╔╝" GREEN "██╔╝ ██╗\n" RESET);
    printf(RED "╚══════╝" BLACK "╚══════╝" BLACK "   ╚═╝   " WHITE "╚═╝  ╚═╝" WHITE "╚═╝     " GREEN " ╚═════╝ " GREEN "╚═╝  ╚═╝\n" RESET);
    printf("\n");
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

int	main(int argc, char **argv, char **env)
{
	t_term	term;
	int		exit_status;

	(void)argv;
	exit_status = 0;
	check_arguments(argc);
	init_terminal(&term);
	setup_interactive_signals();
	print_banner();
	readline_loop(term, env, &exit_status);
	return (0);
}
