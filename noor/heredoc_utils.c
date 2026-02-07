/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:15:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/07 13:02:42 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	heredoc_sigint_handler(int sig)
{
	if (sig == SIGINT)
		exit(1);
}

int	handle_heredoc_line(t_heredoc_line_data *data, int write_fd, char *line)
{
	char	*expanded_line;

	if (ft_strcmp(line, data->redir->file) == 0)
	{
		free(line);
		return (1);
	}
	if (data->redir->quote_type == 0)
	{
		expanded_line = expand_one_arg(line, data->env, data->cmd);
		ft_putstr_fd(expanded_line, write_fd);
		ft_putchar_fd('\n', write_fd);
		free(expanded_line);
		free(line);
	}
	else
	{
		ft_putstr_fd(line, write_fd);
		ft_putchar_fd('\n', write_fd);
		free(line);
	}
	return (0);
}

void	heredoc_child(t_redir *redir, int write_fd, t_list *env, t_cmd *cmd)
{
	char				*line;
	t_heredoc_line_data	data;

	data.redir = redir;
	data.env = env;
	data.cmd = cmd;
	signal(SIGINT, heredoc_sigint_handler);
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(2, "minishell: warning: here-document delimited by ", 47);
			write(2, "end-of-file (wanted `", 21);
			write(2, redir->file, ft_strlen(redir->file));
			write(2, "')\n", 3);
			break ;
		}
		if (handle_heredoc_line(&data, write_fd, line))
			break ;
	}
	close(write_fd);
	exit(0);
}

int	handle_heredoc_fork_error(t_heredoc_context *context)
{
	perror("minishell");
	close(context->fd[0]);
	close(context->fd[1]);
	signal(SIGINT, context->old_sigint_handler);
	return (-1);
}

int	redirection_heredoc(t_redir *redirs)
{
	if (redirs->heredoc_fd == -1)
	{
		ft_putstr_fd("minishell: heredoc fd not initialized\n", 2);
		return (-1);
	}
	if (dup2(redirs->heredoc_fd, STDIN_FILENO) == -1)
	{
		perror("minishell");
		return (-1);
	}
	close(redirs->heredoc_fd);
	return (0);
}
