/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 14:27:53 by njaradat          #+#    #+#             */
/*   Updated: 2026/02/02 15:43:12 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"



// Loop through all commands
// - Skip `EXEC_BUILTIN_PARENT` (already executed)
// - Fork for `EXEC_BUILTIN_CHILD` and `EXEC_EXTERNAL`
// - In child:
//   1. Connect pipes first
//   2. Apply redirections (overrides pipes)
//   3. Close ALL pipe fds
//   4. Restore signals (SIGINT, SIGQUIT to SIG_DFL)
//   5. Execute builtin or external command
// - In parent:
//   - Store PID
//   - Never call dup2 or execve
//   - Continue loop
// - After loop:
//   - Parent closes all pipes
//   - Wait for all children
//   - Last command's exit status becomes `$?`


static void execute_external(t_cmd *cmd, t_list *env)
{
    char *path;
    char **envp;

    // Find command path
    path = find_cmd_path(cmd->argv[0], env);
    if (!path)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(cmd->argv[0], 2);
        ft_putstr_fd(": command not found\n", 2);
        exit(127);
    }
    
    // Convert env to array
    envp = env_list_to_array(env);
    if (!envp)
    {
        free(path);
        exit(1);
    }
    
    // Execute (never returns on success)
    execve(path, cmd->argv, envp);
    
    // execve failed
    perror("minishell");
    free(path);
    free_env_array(envp);
    
    if (errno == EACCES)//check this IDK what is this
        exit(126);  // Permission denied
    exit(127);      // Command not found or other error
}

static void wait_all_children(t_execution *exec)
{
    int i;
    int status;
    int last_status;
    
    // Ignore SIGINT while waiting for children
    signal(SIGINT, SIG_IGN);
    
    i = 0;
    last_status = 0;
    while (i < exec->cmd_count)
    {
        // Skip if no child forked (EXEC_BUILTIN_PARENT or fork failed)
        if (exec->pids[i] <= 0)
        {
            i++;
            continue;
        }
        
        // Wait for this child
        waitpid(exec->pids[i], &status, 0);
        
        // Extract exit status
        if (WIFEXITED(status))
            last_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
        {
            last_status = 128 + WTERMSIG(status);
            // Print newline when child is killed by signal (like Ctrl+C)
            if (WTERMSIG(status) == SIGINT)
                write(1, "\n", 1);
        }
        i++;
    }
    
    // Restore SIGINT handler
    signal_handle();
    
    // Update execution status (last command's exit status becomes $?)
    exec->last_status = last_status;
    g_exit_status = last_status;
}

// Main execution function - fork and execute all commands
void execute_commands(t_execution *exec, t_list **env)
{
    int i;
    t_cmd *cmd;
    pid_t pid;
    
    i = 0;
    cmd = exec->cmd_list;
    while (cmd && i < exec->cmd_count)
    {
        // Step 1: Skip builtins already executed in parent
        if (exec->types[i] == EXEC_BUILTIN_PARENT)
        {
            cmd = cmd->next;
            i++;
            continue;
        }
        
        // Step 2: Fork
        pid = fork();
        if (pid == -1)
        {
            perror("minishell: fork");
            exec->pids[i] = -1;  // Mark as failed
            cmd = cmd->next;
            i++;
            continue;
        }
        
        if (pid == 0)  // Child process
        {
            // Step 3a: Connect pipes (stdin/stdout)
            if (exec->pipes)
                connect_pipes_for_child(i, exec->pipes, exec->cmd_count,
                                       cmd->pipe_in, cmd->pipe_out);
            
            // Step 3b: Apply redirections (overrides pipes)
            if (apply_redirections(cmd->redirs) == -1)
                exit(1);
            
            // Step 3c: Close ALL pipe fds in child
            if (exec->pipes)
                close_all_pipes(exec->pipes, exec->cmd_count - 1);
            
            // Step 3d: Restore default signal handlers
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            
            // Step 3e: Execute
            if (exec->types[i] == EXEC_BUILTIN_CHILD)
                exit(execute_builtin(cmd, env));
            else  // EXEC_EXTERNAL
                execute_external(cmd, *env);  // Never returns
        }
        
        // Step 4: Parent process
        exec->pids[i] = pid;  // Store child PID
        cmd = cmd->next;
        i++;
    }
    
    // Step 5: Parent closes all pipes (after all forks)
    if (exec->pipes)
        close_all_pipes(exec->pipes, exec->cmd_count - 1);
    
    // Step 6: Wait for all children
    wait_all_children(exec);
}


void	execution(t_cmd *cmds, char **env)
{
	t_execution	*exec;
	static t_list *envc = NULL;

	// Initialize environment only once
	if (!envc)
		envc = env_copy(env);
	expander(cmds, envc);
	if (!cmds)
		return ;
	exec = preparation(cmds);
	if (!exec)
		return ;
	execute_builtins_parent(exec, &envc);
	execute_commands(exec, &envc);
	g_exit_status = exec->last_status;
	free_execs(exec);
}