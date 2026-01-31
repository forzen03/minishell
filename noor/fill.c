// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   fill.c                                             :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2026/01/17 00:00:00 by noorjaradat       #+#    #+#             */
// /*   Updated: 2026/01/27 16:06:07 by njaradat         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../minishell.h"
// #include <stdio.h>
// #include <stdlib.h>

// // Helper to create argv array from variable number of arguments
// static char **make_argv(const char **args)
// {
//     char **argv;
//     int count;
//     int i;

//     count = 0;
//     while (args[count])
//         count++;
//     argv = (char **)malloc(sizeof(char *) * (count + 1));
//     if (!argv)
//         return (NULL);
//     i = 0;
//     while (i < count)
//     {
//         argv[i] = ft_strdup(args[i]);
//         if (!argv[i])
//         {
//             while (i > 0)
//                 free(argv[--i]);
//             free(argv);
//             return (NULL);
//         }
//         i++;
//     }
//     argv[count] = NULL;
//     return (argv);
// }

// // Create a single command node
// t_cmd *create_cmd(const char **args, int pipe_in, int pipe_out)
// {
//     t_cmd *cmd;

//     cmd = (t_cmd *)malloc(sizeof(t_cmd));
//     if (!cmd)
//         return (NULL);
//     ft_bzero(cmd, sizeof(t_cmd));
//     cmd->argv = make_argv(args);
//     if (!cmd->argv)
//     {
//         free(cmd);
//         return (NULL);
//     }
//     cmd->redirs = NULL;
//     cmd->pipe_in = pipe_in;
//     cmd->pipe_out = pipe_out;
//     cmd->next = NULL;
//     return (cmd);
// }

// // Free command list
// void free_cmd_list(t_cmd *cmd)
// {
//     t_cmd *tmp;
//     int i;

//     while (cmd)
//     {
//         tmp = cmd->next;
//         if (cmd->argv)
//         {
//             i = 0;
//             while (cmd->argv[i])
//                 free(cmd->argv[i++]);
//             free(cmd->argv);
//         }
//         free(cmd);
//         cmd = tmp;
//     }
// }

// // Print execution plan
// static void print_execution_plan(t_execution *exec)
// {
//     t_cmd *cmd;
//     int i;
//     const char *type_str;

//     printf("\n=== Execution Plan ===\n");
//     printf("Total commands: %d\n", exec->cmd_count);
    
//     if (exec->cmd_count > 1)
//     {
//         printf("Pipes created: %d\n", exec->cmd_count - 1);
//         i = 0;
//         while (i < exec->cmd_count - 1)
//         {
//             printf("  pipe[%d]: read_fd=%d, write_fd=%d\n", 
//                    i, exec->pipes[i][0], exec->pipes[i][1]);
//             i++;
//         }
//     }
    
//     printf("\nCommands:\n");
//     cmd = exec->cmd_list;
//     i = 0;
//     while (cmd)
//     {
//         type_str = (exec->types[i] == EXEC_BUILTIN_PARENT) ? "BUILTIN_PARENT" :
//                    (exec->types[i] == EXEC_BUILTIN_CHILD) ? "BUILTIN_CHILD" :
//                    "EXTERNAL";
        
//         printf("[%d] %s (type: %s)\n", i, cmd->argv[0], type_str);
//         printf("    pipe_in=%d, pipe_out=%d\n", cmd->pipe_in, cmd->pipe_out);
        
//         if (cmd->pipe_in && i > 0)
//             printf("    stdin  <- pipe[%d][0] (fd %d)\n", 
//                    i-1, exec->pipes[i-1][0]);
//         else
//             printf("    stdin  <- terminal (fd 0)\n");
            
//         if (cmd->pipe_out && i < exec->cmd_count - 1)
//             printf("    stdout -> pipe[%d][1] (fd %d)\n", 
//                    i, exec->pipes[i][1]);
//         else
//             printf("    stdout -> terminal (fd 1)\n");
        
//         cmd = cmd->next;
//         i++;
//     }
//     printf("\n");
// }

// int main(void)
// {
//     t_cmd *cmd1, *cmd2, *cmd3;
//     t_execution *exec;
    
//     printf("=== Test 1: Single command (ls -l) ===\n");
//     const char *args1[] = {"ls", "-l", NULL};
//     cmd1 = create_cmd(args1, 0, 0);
//     exec = preparation(cmd1);
//     if (exec)
//     {
//         print_execution_plan(exec);
//         free_execs(exec);
//     }
//     free_cmd_list(cmd1);
    
//     printf("\n=== Test 2: Two commands (echo hello | wc -l) ===\n");
//     const char *args2[] = {"echo", "hello", NULL};
//     const char *args3[] = {"wc", "-l", NULL};
//     cmd1 = create_cmd(args2, 0, 1);
//     cmd2 = create_cmd(args3, 1, 0);
//     cmd1->next = cmd2;
//     exec = preparation(cmd1);
//     if (exec)
//     {
//         print_execution_plan(exec);
//         free_execs(exec);
//     }
//     free_cmd_list(cmd1);
    
//     printf("\n=== Test 3: Three commands (ls | grep txt | wc -l) ===\n");
//     const char *args4[] = {"ls", NULL};
//     const char *args5[] = {"grep", "txt", NULL};
//     const char *args6[] = {"wc", "-l", NULL};
//     cmd1 = create_cmd(args4, 0, 1);
//     cmd2 = create_cmd(args5, 1, 1);
//     cmd3 = create_cmd(args6, 1, 0);
//     cmd1->next = cmd2;
//     cmd2->next = cmd3;
//     exec = preparation(cmd1);
//     if (exec)
//     {
//         print_execution_plan(exec);
//         free_execs(exec);
//     }
//     free_cmd_list(cmd1);
    
//     printf("\n=== Test 4: Builtin alone (cd /tmp) ===\n");
//     const char *args7[] = {"cd", "/tmp", NULL};
//     cmd1 = create_cmd(args7, 0, 0);
//     exec = preparation(cmd1);
//     if (exec)
//     {
//         print_execution_plan(exec);
//         free_execs(exec);
//     }
//     free_cmd_list(cmd1);
    
//     printf("\n=== Test 5: Builtin in pipeline (echo test | pwd) ===\n");
//     const char *args8[] = {"echo", "test", NULL};
//     const char *args9[] = {"pwd", NULL};
//     cmd1 = create_cmd(args8, 0, 1);
//     cmd2 = create_cmd(args9, 1, 0);
//     cmd1->next = cmd2;
//     exec = preparation(cmd1);
//     if (exec)
//     {
//         print_execution_plan(exec);
//         free_execs(exec);
//     }
//     free_cmd_list(cmd1);
    
//     return (0);
// }
