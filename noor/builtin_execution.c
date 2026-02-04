/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 16:13:24 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/04 17:36:47 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

int execute_builtin(t_cmd *cmd, t_list **env, int *exit_status)
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

int execute_builtins_parent(t_execution *exec, t_list **env, int *exit_status)
{
    t_cmd *cmd;
    int i;
    int status;
    int saved_stdin;
    int saved_stdout;

    status = 0;
    cmd = exec->cmd_list;
    i = 0;
    while (cmd && i < exec->cmd_count)
    {
        if (exec->types[i] == EXEC_BUILTIN_PARENT)
        {
            // Save original stdin/stdout
            saved_stdin = dup(STDIN_FILENO);
            saved_stdout = dup(STDOUT_FILENO);
            
            if (saved_stdin == -1 || saved_stdout == -1)
            {
                if (saved_stdin != -1)
                    close(saved_stdin);
                if (saved_stdout != -1)
                    close(saved_stdout);
                perror("minishell: dup");
                status = 1;
                cmd = cmd->next;
                i++;
                continue;
            }
            
            // Apply redirections
            if (apply_redirections(cmd->redirs) == -1)
            {
                status = 1;
                // Restore stdin/stdout
                dup2(saved_stdin, STDIN_FILENO);
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdin);
                close(saved_stdout);
                cmd = cmd->next;
                i++;
                continue;
            }
            
            // Execute the builtin
            status = execute_builtin(cmd, env, exit_status);
            
            // Restore stdin/stdout
            dup2(saved_stdin, STDIN_FILENO);
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdin);
            close(saved_stdout);
            
            // If it was exit and it returned (too many args), continue
            // If exit succeeded, we won't reach here
        }
        cmd = cmd->next;
        i++;
    }
    exec->last_status = status;
    return (status);
}
