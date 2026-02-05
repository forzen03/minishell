/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 15:00:47 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 15:00:49 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	connect_pipes_for_child(int cmd_idx, t_execution *exec, t_cmd *cmd)
{
	if (cmd->pipe_in && cmd_idx > 0)
	{
		if (dup2(exec->pipes[cmd_idx - 1][0], STDIN_FILENO) == -1)
			return (-1);
	}
	if (cmd->pipe_out && cmd_idx < exec->cmd_count - 1)
	{
		if (dup2(exec->pipes[cmd_idx][1], STDOUT_FILENO) == -1)
			return (-1);
	}
	return (0);
}

void	close_all_pipes(int **pipes, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}
