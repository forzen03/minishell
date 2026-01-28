/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exapnder2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 17:24:28 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int get_new_len(t_cmd *cmd)
{
	int i;
	int len;

	len = 0;
	i = 0;

	while (cmd->argv && cmd->argv[i])
	{

		if (cmd->argv[i][0] != '\0' || cmd->quote_types[i] != 0)
			len++;
		i++;
	}
	return (len);
}

void rebuild_argv(t_cmd *cmd, t_cmd *cmds, t_list *env)
{
	int argc;
	char **new_argv;
	int *new_quote_types;

	argc = 0;
	argc = get_new_len(cmds);
	new_argv = malloc(sizeof(char *) * (argc + 1));

	if (!new_argv)
		memory_allocation_failed_expand(cmds, env);
	new_quote_types = malloc(sizeof(int) * argc);

	if (!new_quote_types)
	{
		free(new_argv);
		memory_allocation_failed_expand(cmds, env);
	}
	rebuild_argv_loop(cmd, new_argv, new_quote_types);
	new_argv[argc] = NULL;
	free(cmd->argv);
	free(cmd->quote_types);
	cmd->argv = new_argv;
	cmd->quote_types = new_quote_types;
}

void expander(t_cmd *cmds, t_list *env)
{
	int i;
	t_cmd *tmp;
	char *new;

	tmp = cmds;

	while (tmp)
	{
		i = 0;

		while (tmp->argv && tmp->argv[i])
		{
			new = expand_one_arg(tmp->argv[i], env, tmp);

			if (!new)
				new = ft_strdup("");
			free(tmp->argv[i]);
			tmp->argv[i] = new;
			i++;
		}
		rebuild_argv(tmp, cmds, env);
		expand_redirection(tmp, env);
		tmp = tmp->next;
	}
}
