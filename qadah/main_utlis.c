/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utlis.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 15:40:51 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 15:41:11 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_two_strings(char *s1, char *s2)
{
	free(s1);
	free(s2);
}

int	minishell_cycle(char *line, char **env, int *exit_status)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = tokenizer(line);
	if (!tokens)
		return (0);
	cmds = parser(tokens);
	if (!cmds)
	{
		tokens_clear(&tokens, free);
		cmds_cleaner(cmds);
		*exit_status = 2;
		return (1);
	}
	cmds->exit_status = *exit_status;
	tokens_clear(&tokens, free);
	execution(cmds, env, exit_status);
	cmds->exit_status = *exit_status;
	cmds_cleaner(cmds);
	return (0);
}

int	check_quotes_loop(char *line, int *exit_status)
{
	if (check_quotes(line))
	{
		write(2, "Syntax error: unclosed quotes\n", 31);
		free(line);
		*exit_status = 2;
		return (1);
	}
	return (0);
}

void	control_c_exit_status(int *exit_status)
{
	*exit_status = 130;
	g_signal = 0;
}

void	readline_loop(t_term term, char **env, int *exit_status)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (g_signal == SIGINT)
			control_c_exit_status(exit_status);
		if (!line)
			exit_handle(&term, *exit_status);
		if (!*line)
		{
			free(line);
			continue ;
		}
		if (*line && not_all_space(line))
			add_history(line);
		if (check_quotes_loop(line, exit_status))
			continue ;
		if (minishell_cycle(line, env, exit_status))
		{
			free(line);
			continue ;
		}
		free(line);
	}
}
