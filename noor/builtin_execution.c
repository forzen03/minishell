/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 16:13:24 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 13:17:25 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_builtin(t_cmd *cmd, t_list **env, int *exit_status)
{
	if (!cmd || !cmd->argv[0])
		return (1);
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (builtin_cd(cmd->argv, env));
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (builtin_export(cmd->argv, env));
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (builtin_unset(cmd->argv, env));
	else if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (builtin_exit(cmd->argv, exit_status));
	else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (builtin_pwd(env));
	else if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (builtin_echo(cmd->argv));
	else if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (builtin_env(*env));
	return (1);
}

static void	handle_builtin_parent_dup(int saved_stdin, int saved_stdout)
{
	if (saved_stdin != -1)
		close(saved_stdin);
	if (saved_stdout != -1)
		close(saved_stdout);
	perror("minishell: dup");
}

static int	handle_builtin_parent_cmd(t_cmd *cmd, t_list **env,
		int *exit_status)
{
	int	saved_stdin;
	int	saved_stdout;
	int	status;

	status = 0;
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		handle_builtin_parent_dup(saved_stdin, saved_stdout);
		return (1);
	}
	if (apply_redirections(cmd->redirs) == -1)
		status = 1;
	else
		status = execute_builtin(cmd, env, exit_status);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (status);
}

int	execute_builtins_parent(t_execution *exec, t_list **env, int *exit_status)
{
	t_cmd	*cmd;
	int		i;
	int		status;

	status = 0;
	cmd = exec->cmd_list;
	i = 0;
	while (cmd && i < exec->cmd_count)
	{
		if (exec->types[i] == EXEC_BUILTIN_PARENT)
			status = handle_builtin_parent_cmd(cmd, env, exit_status);
		cmd = cmd->next;
		i++;
	}
	exec->last_status = status;
	return (status);
}
