/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 16:13:24 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/01/29 12:25:47 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

int execute_builtin(t_cmd *cmd, t_list ***env)
{
    if (!cmd || !cmd->argv[0])
        return (1);
    
    if (ft_strcmp(cmd->argv[0], "cd") == 0)
        return (builtin_cd(cmd->argv, **env));
    else if (ft_strcmp(cmd->argv[0], "export") == 0)
        return (builtin_export(cmd->argv, *env));
    else if (ft_strcmp(cmd->argv[0], "unset") == 0)
        return (builtin_unset(cmd->argv, *env));
    else if (ft_strcmp(cmd->argv[0], "exit") == 0)
        return (builtin_exit(cmd->argv));
    else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
        return (builtin_pwd(*env));
    else if (ft_strcmp(cmd->argv[0], "echo") == 0)
        return (builtin_echo(cmd->argv));
    else if (ft_strcmp(cmd->argv[0], "env") == 0)
        return (builtin_env(**env));
    return (1);
}

int execute_builtins_parent(t_execution *exec,t_list ***env)
{
    t_cmd *cmd;
    int i;
    int status;

    status = 0;
    cmd = exec->cmd_list;
    i = 0;
    while (cmd && i < exec->cmd_count)
    {
        if (exec->types[i] == EXEC_BUILTIN_PARENT)
        {
            status = execute_builtin(cmd,env);
        }
        cmd = cmd->next;
        i++;
    }
    exec->last_status = status;
    return (status);
}