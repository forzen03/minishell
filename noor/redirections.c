/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 12:05:32 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 14:35:52 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	apply_heredoc_redirection(t_redir *redirs)
{
	if (redirs->heredoc_fd != -1)
	{
		if (redirection_heredoc(redirs) != 0)
			return (-1);
	}
	return (0);
}

int	redirection_in(t_redir *redirs)
{
	int	fd;

	fd = open(redirs->file, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		perror("minishell");
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirection_out(t_redir *redirs)
{
	int	fd;

	fd = open(redirs->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		perror("minishell");
		return (-1);
	}
	close(fd);
	return (0);
}

int	redirection_append(t_redir *redirs)
{
	int	fd;

	fd = open(redirs->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("minishell");
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		perror("minishell");
		return (-1);
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_redir *redirs)
{
	while (redirs)
	{
		if (redirs->type == TOKEN_REDIR_IN)
		{
			if (redirection_in(redirs) != 0)
				return (-1);
		}
		else if (redirs->type == TOKEN_REDIR_OUT)
		{
			if (redirection_out(redirs) != 0)
				return (-1);
		}
		else if (redirs->type == TOKEN_REDIR_APPEND)
		{
			if (redirection_append(redirs) != 0)
				return (-1);
		}
		else if (redirs->type == TOKEN_HEREDOC)
		{
			if (apply_heredoc_redirection(redirs) != 0)
				return (-1);
		}
		redirs = redirs->next;
	}
	return (0);
}
