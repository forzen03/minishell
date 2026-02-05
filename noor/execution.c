/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:27:53 by njaradat          #+#    #+#             */
/*   Updated: 2026/02/05 17:25:02 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	wait_all_children(t_execution *exec)
{
	int	i;
	int	status;
	int	last_status;
	int	had_children;

	setup_execution_signals();
	i = 0;
	last_status = 0;
	had_children = 0;
	while (i < exec->cmd_count)
	{
		if (exec->pids[i] <= 0)
		{
			i++;
			continue ;
		}
		had_children = 1;
		waitpid(exec->pids[i], &status, 0);
		last_status = process_child_status(status);
		i++;
	}
	setup_interactive_signals();
	if (had_children)
		exec->last_status = last_status;
}

static void	process_fork_iteration(t_fork_data *data, t_cmd **cmd, int *i)
{
	pid_t	pid;

	if (data->exec->types[*i] == EXEC_BUILTIN_PARENT)
	{
		*cmd = (*cmd)->next;
		(*i)++;
		return ;
	}
	data->i = *i;
	pid = fork();
	if (handle_fork_result(pid, data) == -1)
	{
		*cmd = (*cmd)->next;
		(*i)++;
		return ;
	}
	data->exec->pids[*i] = pid;
	*cmd = (*cmd)->next;
	(*i)++;
}

static void	fork_and_execute(t_execution *exec, t_list **env, int *exit_status)
{
	int			i;
	t_cmd		*cmd;
	t_fork_data	data;

	data.exec = exec;
	data.env = env;
	data.exit_status = exit_status;
	i = 0;
	cmd = exec->cmd_list;
	while (cmd && i < exec->cmd_count)
	{
		process_fork_iteration(&data, &cmd, &i);
	}
}

void	execute_commands(t_execution *exec, t_list **env, int *exit_status)
{
	fork_and_execute(exec, env, exit_status);
	if (exec->pipes)
		close_all_pipes(exec->pipes, exec->cmd_count - 1);
	wait_all_children(exec);
}

void	execution(t_cmd *cmds, char **env, int *exit_status)
{
	t_execution		*exec;
	static t_list	*envc = NULL;

	if (!envc)
		envc = env_copy(env);
	expander(cmds, envc);
	if (!cmds)
		return ;
	if (process_heredocs(cmds, envc) == -1)
	{
		*exit_status = 130;
		return ;
	}
	exec = preparation(cmds);
	if (!exec)
		return ;
	execute_builtins_parent(exec, &envc, exit_status);
	execute_commands(exec, &envc, exit_status);
	*exit_status = exec->last_status;
	close_heredoc_fds(cmds);
	free_execs(exec);
}
