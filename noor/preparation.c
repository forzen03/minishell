/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparation.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 14:12:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 14:11:16 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_builtin(t_cmd *c)
{
	char	*cmd;

	if (!c || !c->argv[0] || !c->argv[0][0])
		return (0);
	cmd = c->argv[0];
	if (!(ft_strcmp(cmd, "echo")))
		return (1);
	if (!(ft_strcmp(cmd, "cd")))
		return (1);
	if (!(ft_strcmp(cmd, "pwd")))
		return (1);
	if (!(ft_strcmp(cmd, "export")))
		return (1);
	if (!(ft_strcmp(cmd, "unset")))
		return (1);
	if (!(ft_strcmp(cmd, "env")))
		return (1);
	if (!(ft_strcmp(cmd, "exit")))
		return (1);
	return (0);
}

static int	is_in_pipeline(t_cmd *c)
{
	return (c->pipe_in == 1 || c->pipe_out == 1);
}

static t_exec_type	get_exec_type(t_cmd *cmd)
{
	int	is_built;
	int	in_pipeline;

	is_built = is_builtin(cmd);
	in_pipeline = is_in_pipeline(cmd);
	if (is_built && !in_pipeline)
		return (EXEC_BUILTIN_PARENT);
	else if (is_built && in_pipeline)
		return (EXEC_BUILTIN_CHILD);
	else
		return (EXEC_EXTERNAL);
}

static t_exec_type	*create_exec_types(t_cmd *cmd, int cmd_count)
{
	t_exec_type	*types;
	int			i;

	types = malloc(sizeof(t_exec_type) * cmd_count);
	if (!types)
		return (NULL);
	ft_bzero(types, sizeof(t_exec_type) * cmd_count);
	i = 0;
	while (cmd && i < cmd_count)
	{
		types[i] = get_exec_type(cmd);
		cmd = cmd->next;
		i++;
	}
	return (types);
}

t_execution	*preparation(t_cmd *c)
{
	t_execution	*exec;

	if (!c)
		return (NULL);
	exec = malloc(sizeof(t_execution));
	if (!exec)
		return (NULL);
	ft_bzero(exec, sizeof(t_execution));
	exec->cmd_list = c;
	exec->cmd_count = count_cmds(c);
	exec->pids = malloc(sizeof(pid_t) * exec->cmd_count);
	if (!exec->pids)
		return (free_execs(exec));
	ft_bzero(exec->pids, sizeof(pid_t) * exec->cmd_count);
	exec->types = create_exec_types(c, exec->cmd_count);
	if (!exec->types)
		return (free_execs(exec));
	if (exec->cmd_count > 1)
	{
		exec->pipes = create_pipes(exec->cmd_count - 1);
		if (!exec->pipes)
			return (free_execs(exec));
	}
	return (exec);
}
