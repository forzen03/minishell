
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handling.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/17 00:00:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/01/17 00:00:00 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/*
** Stage 6: Pipe handling helpers
** - These functions DO NOT fork or exec.
** - They only help wire stdin/stdout between commands.
** - Call them from the child process setup during Stage 8.
*/

/*
Connect a command to the pipeline inside the CHILD process.
- cmd_idx: index of the command in the pipeline (0..cmd_count-1)
- pipes:   array of pipe fd pairs, length = cmd_count - 1
- cmd_count: total number of commands
- pipe_in/pipe_out: flags from t_cmd indicating whether the command
- should read from previous pipe and/or write to next pipe.
- Returns 0 on success, -1 on error (dup2 failure).
*/
int connect_pipes_for_child(int cmd_idx, int **pipes, int cmd_count,
                            int pipe_in, int pipe_out)
{
    if (pipe_in && cmd_idx > 0)
    {
        if (dup2(pipes[cmd_idx - 1][0], STDIN_FILENO) == -1)
            return (-1);
    }
    if (pipe_out && cmd_idx < cmd_count - 1)
    {
        if (dup2(pipes[cmd_idx][1], STDOUT_FILENO) == -1)
            return (-1);
    }
    return (0);
}

/*
Close ALL pipe file descriptors.
- Use in the child after dup2() to drop original fds.
- Use in the parent after the fork loop to release pipe fds.
*/
void close_all_pipes(int **pipes, int pipe_count)
{
    int i;

    i = 0;
    while (i < pipe_count)
    {
        close(pipes[i][0]);
        close(pipes[i][1]);
        i++;
    }
}