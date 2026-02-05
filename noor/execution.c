/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:02:02 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 15:21:07 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute_external(t_cmd *cmd, t_list *env)
{
	char	*path;
	char	**envp;

	path = find_cmd_path(cmd->argv[0], env);
	if (!path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		exit(127);
	}
	envp = env_list_to_array(env);
	if (!envp)
	{
		free(path);
		exit(1);
	}
	execve(path, cmd->argv, envp);
	perror("minishell");
	free(path);
	free_env_array(envp);
	if (errno == EACCES)
		exit(126);
	exit(127);
}

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
		last_status = child_status(status);
		i++;
	}
	setup_interactive_signals();
	if (had_children)
		exec->last_status = last_status;
}

static void	process_command_iteration(t_execution *exec, t_cmd *cmd, int i,
		t_list **env)
{
	pid_t	pid;

	if (exec->types[i] == EXEC_BUILTIN_PARENT)
		return ;
	pid = fork();
	if (pid == -1)
	{
		perror("minishell: fork");
		exec->pids[i] = -1;
		return ;
	}
	if (pid == 0)
		execute_child_command(exec, cmd, i, env);
	exec->pids[i] = pid;
}

void	run_command_pipeline(t_execution *exec, t_list **env)
{
	int		i;
	t_cmd	*cmd;

	i = 0;
	cmd = exec->cmd_list;
	while (cmd && i < exec->cmd_count)
	{
		process_command_iteration(exec, cmd, i, env);
		cmd = cmd->next;
		i++;
	}
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
	run_command_pipeline(exec, &envc);
	*exit_status = exec->last_status;
	close_heredoc_fds(cmds);
	free_execs(exec);
}
