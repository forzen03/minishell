/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:15:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/07 12:48:59 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_heredoc_fork(t_redir *redir, t_list *env, t_cmd *cmd,
		t_heredoc_context *context)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (handle_heredoc_fork_error(context));
	if (pid == 0)
	{
		close(context->fd[0]);
		heredoc_child(redir, context->fd[1], env, cmd);
	}
	close(context->fd[1]);
	waitpid(pid, &status, 0);
	signal(SIGINT, context->old_sigint_handler);
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
	{
		close(context->fd[0]);
		write(1, "\n", 1);
		cmd->exit_status = 130;
		return (-1);
	}
	return (context->fd[0]);
}

int	read_heredoc_content(t_redir *redir, t_list *env, t_cmd *cmd)
{
	t_heredoc_context	context;

	if (pipe(context.fd) == -1)
	{
		perror("minishell");
		return (-1);
	}
	context.old_sigint_handler = signal(SIGINT, SIG_IGN);
	return (handle_heredoc_fork(redir, env, cmd, &context));
}

int	handle_cmd_heredoc(t_cmd *cmd, t_list *env, t_redir *redir)
{
	t_redir	*prev_redir;

	prev_redir = cmd->redirs;
	while (prev_redir != redir)
	{
		if (prev_redir->type == TOKEN_HEREDOC && prev_redir->heredoc_fd > 0)
		{
			close(prev_redir->heredoc_fd);
			prev_redir->heredoc_fd = -1;
		}
		prev_redir = prev_redir->next;
	}
	redir->heredoc_fd = read_heredoc_content(redir, env, cmd);
	if (redir->heredoc_fd == -1)
		return (-1);
	return (0);
}

int	process_cmd_heredocs(t_cmd *cmd, t_list *env)
{
	t_redir	*redir;

	redir = cmd->redirs;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
		{
			if (handle_cmd_heredoc(cmd, env, redir) == -1)
				return (-1);
		}
		else
			redir->heredoc_fd = -1;
		redir = redir->next;
	}
	return (0);
}

int	process_heredocs(t_cmd *cmd_list, t_list *env)
{
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		if (process_cmd_heredocs(cmd, env) == -1)
			return (-1);
		cmd = cmd->next;
	}
	return (0);
}
