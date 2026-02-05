/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:10:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 17:26:44 by noorjaradat      ###   ########.fr       */
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

int	process_child_status(int status)
{
	int	exit_code;

	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
	else
		exit_code = 0;
	return (exit_code);
}

t_cmd	*get_cmd_at_index(t_cmd *cmd_list, int index)
{
	int	i;

	i = 0;
	while (cmd_list && i < index)
	{
		cmd_list = cmd_list->next;
		i++;
	}
	return (cmd_list);
}

void	execute_child_process(t_execution *exec, int i, t_list **env,
		int *exit_status)
{
	t_cmd	*cmd;

	cmd = get_cmd_at_index(exec->cmd_list, i);
	if (!cmd)
		exit(1);
	close_other_heredoc_fds(exec->cmd_list, cmd);
	if (exec->pipes)
		connect_pipes_for_child(exec, cmd, i);
	if (apply_redirections(cmd->redirs) == -1)
		exit(1);
	if (exec->pipes)
		close_all_pipes(exec->pipes, exec->cmd_count - 1);
	restore_default_signals();
	if (exec->types[i] == EXEC_BUILTIN_CHILD)
		exit(execute_builtin(cmd, env, exit_status));
	else
		execute_external(cmd, *env);
}

int	handle_fork_result(pid_t pid, t_fork_data *data)
{
	if (pid == -1)
	{
		perror("minishell: fork");
		data->exec->pids[data->i] = -1;
		return (-1);
	}
	if (pid == 0)
		execute_child_process(data->exec, data->i, data->env,
			data->exit_status);
	return (0);
}
