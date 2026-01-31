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

int		g_exit_status = 0;

void	free_two_strings(char *s1, char *s2)
{
	free(s1);
	free(s2);
}

int	minishell_cycle(char *line, char **env)
{
	t_token	*tokens;
	t_list	*envc;
	t_cmd	*cmds;

	tokens = tokenizer(line);
	if (!tokens)
		return (0);
	envc = env_copy(env);
	cmds = parser(tokens);
	if (!cmds)
	{
		tokens_clear(&tokens, free);
		ft_lstclear(&envc, free);
		cmds_cleaner(cmds);
		return (1);
	}
	tokens_clear(&tokens, free);
	expander(cmds, envc);
	execution(cmds, &envc);
	cmds_cleaner(cmds);
	ft_lstclear(&envc, free);
	return (0);
}

int	check_quotes_loop(char *line)
{
	if (check_quotes(line))
	{
		write(2, "Syntax error: unclosed quotes\n", 31);
		free(line);
		return (1);
	}
	return (0);
}

void	readline_loop(t_term term, char **env)
{
	char	*line;

	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			exit_handle(&term);
		if (!*line)
		{
			free(line);
			continue ;
		}
		if (*line && not_all_space(line))
			add_history(line);
		if (check_quotes_loop(line))
			continue ;
		if (minishell_cycle(line, env))
		{
			free(line);
			continue ;
		}
		free(line);
	}
}

int	main(int argc, char **argv, char **env)
{
	t_term	term;

	(void)argv;
	check_arguments(argc);
	init_terminal(&term);
	signal_handle();
	readline_loop(term, env);
	return (0);
}
