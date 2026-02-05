/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:15:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 17:08:44 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_redirection_heredoc(t_redir *redirs)
{
	if (redirs->heredoc_fd != -1)
	{
		if (redirection_heredoc(redirs) != 0)
			return (-1);
	}
	return (0);
}

void	close_heredoc_fds(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = cmd_list;
	while (cmd)
	{
		redir = cmd->redirs;
		while (redir)
		{
			if (redir->type == TOKEN_HEREDOC && redir->heredoc_fd > 0)
			{
				close(redir->heredoc_fd);
				redir->heredoc_fd = -1;
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

void	close_other_heredoc_fds(t_cmd *cmd_list, t_cmd *current_cmd)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd != current_cmd)
		{
			redir = cmd->redirs;
			while (redir)
			{
				if (redir->type == TOKEN_HEREDOC && redir->heredoc_fd > 0)
				{
					close(redir->heredoc_fd);
					redir->heredoc_fd = -1;
				}
				redir = redir->next;
			}
		}
		cmd = cmd->next;
	}
}
