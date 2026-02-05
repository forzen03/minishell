/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preparation_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 15:45:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 16:41:42 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_cmds(t_cmd *c)
{
	int	i;

	i = 0;
	while (c)
	{
		i++;
		c = c->next;
	}
	return (i);
}

int	**free_pipes(int **pipes, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	return (NULL);
}

int	**create_pipes(int count)
{
	int	**pipes;
	int	i;

	i = 0;
	pipes = malloc((sizeof(int *) * count));
	if (!pipes)
		return (NULL);
	while (i < count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
			return (free_pipes(pipes, count));
		if (pipe(pipes[i]) == -1)
			return (free_pipes(pipes, count));
		i++;
	}
	return (pipes);
}

t_execution	*free_execs(t_execution *e)
{
	if (!e)
		return (NULL);
	if (e->pipes)
		free_pipes(e->pipes, e->cmd_count - 1);
	if (e->pids)
		free(e->pids);
	if (e->types)
		free(e->types);
	free(e);
	return (NULL);
}
