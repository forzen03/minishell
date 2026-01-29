# STAGE 4: FORK & EXECUTION - Complete Implementation Guide

## Overview
This is the final execution stage where we fork child processes and execute commands. This stage brings everything together - pipes, redirections, builtins, and external commands.

---

## What You Already Have (Stages 1-3)
✅ `preparation()` - Counts commands, allocates pipes, determines exec types  
✅ `connect_pipes_for_child()` - Connects stdin/stdout to pipes in child  
✅ `close_all_pipes()` - Closes all pipe file descriptors  
✅ `execute_builtin()` - Executes builtins (both parent and child)  
✅ `execute_builtins_parent()` - Executes parent-only builtins  

---

## What You Need to Implement

### File 1: `noor/redirections.c`
**Purpose:** Handle file redirections (`<`, `>`, `>>`)

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by njaradat          #+#    #+#             */
/*   Updated: 2026/01/29 00:00:00 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int apply_redirections(t_redir *redirs)
{
    int fd;
    
    while (redirs)
    {
        if (redirs->type == TOKEN_REDIR_IN)  // <
        {
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
        }
        else if (redirs->type == TOKEN_REDIR_OUT)  // >
        {
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
        }
        else if (redirs->type == TOKEN_REDIR_APPEND)  // >>
        {
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
        }
        // TOKEN_HEREDOC: Skip for now (not implemented in parser yet)
        
        redirs = redirs->next;
    }
    return (0);
}
```

**Key Points:**
- Loop through all redirections in the list
- Open file with appropriate flags:
  - `<` : O_RDONLY (read only)
  - `>` : O_CREAT | O_WRONLY | O_TRUNC (create, write, truncate)
  - `>>`: O_CREAT | O_WRONLY | O_APPEND (create, write, append)
- Use `dup2()` to redirect stdin/stdout
- Always close the original fd after dup2
- Return -1 on error (child will exit(1))

---

### File 2: `noor/path_utils.c`
**Purpose:** Find command in PATH or handle absolute/relative paths

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by njaradat          #+#    #+#             */
/*   Updated: 2026/01/29 00:00:00 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Get PATH value from environment
static char *get_path_from_env(t_list *env)
{
    while (env)
    {
        if (ft_strncmp((char *)env->content, "PATH=", 5) == 0)
            return ((char *)env->content + 5);
        env = env->next;
    }
    return (NULL);
}

// Free paths array from ft_split
static void free_paths_array(char **paths, int start_index)
{
    int i;
    
    i = start_index;
    while (paths[i])
    {
        free(paths[i]);
        i++;
    }
    free(paths);
}

// Find command in PATH or return absolute/relative path
char *find_command_path(char *cmd, t_list *env)
{
    char **paths;
    char *full_path;
    char *temp;
    char *path_env;
    int i;
    
    if (!cmd || !cmd[0])
        return (NULL);
    
    // If cmd contains '/', it's absolute or relative path
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    
    // Get PATH from environment
    path_env = get_path_from_env(env);
    if (!path_env)
        return (NULL);
    
    // Split PATH by ':'
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    
    // Try each directory
    i = 0;
    while (paths[i])
    {
        // Build full_path = dir + "/" + cmd
        temp = ft_strjoin(paths[i], "/");
        if (!temp)
        {
            free_paths_array(paths, 0);
            return (NULL);
        }
        full_path = ft_strjoin(temp, cmd);
        free(temp);
        if (!full_path)
        {
            free_paths_array(paths, 0);
            return (NULL);
        }
        
        // Check if executable
        if (access(full_path, X_OK) == 0)
        {
            free_paths_array(paths, 0);
            return (full_path);
        }
        free(full_path);
        i++;
    }
    
    // Not found - free paths array
    free_paths_array(paths, 0);
    return (NULL);
}
```

**Key Points:**
- If command contains `/`, treat as absolute/relative path
- Otherwise search in PATH directories
- Use `access(path, X_OK)` to check if executable
- Return allocated string or NULL
- Caller must free the returned path

---

### File 3: `noor/env_utils.c`
**Purpose:** Convert environment list to array for execve

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by njaradat          #+#    #+#             */
/*   Updated: 2026/01/29 00:00:00 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Convert t_list env to char ** array for execve
char **env_list_to_array(t_list *env)
{
    char **envp;
    int count;
    int i;
    t_list *tmp;
    
    // Count environment variables
    count = 0;
    tmp = env;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    
    // Allocate array (count + 1 for NULL terminator)
    envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return (NULL);
    
    // Copy each string
    i = 0;
    while (env && i < count)
    {
        envp[i] = ft_strdup((char *)env->content);
        if (!envp[i])
        {
            // Free previous allocations
            while (i > 0)
                free(envp[--i]);
            free(envp);
            return (NULL);
        }
        env = env->next;
        i++;
    }
    envp[count] = NULL;  // NULL-terminate array
    return (envp);
}

// Free environment array
void free_env_array(char **envp)
{
    int i;
    
    if (!envp)
        return;
    i = 0;
    while (envp[i])
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}
```

**Key Points:**
- Count nodes in env list
- Allocate `char **` array (size + 1 for NULL)
- Copy each string with `strdup`
- NULL-terminate the array
- Free function cleans up everything

---

### File 4: `noor/execution.c`
**Purpose:** Main fork and execution loop

```c
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/29 00:00:00 by njaradat          #+#    #+#             */
/*   Updated: 2026/01/29 00:00:00 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Execute external command with execve
static void execute_external(t_cmd *cmd, t_list *env)
{
    char *path;
    char **envp;
    
    // Find command path
    path = find_command_path(cmd->argv[0], env);
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
    
    if (errno == EACCES)
        exit(126);  // Permission denied
    exit(127);      // Command not found or other error
}

// Wait for all child processes
static void wait_all_children(t_execution *exec)
{
    int i;
    int status;
    int last_status;
    
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
            last_status = 128 + WTERMSIG(status);
        
        i++;
    }
    
    // Update execution status (last command's exit status becomes $?)
    exec->last_status = last_status;
    g_exit_status = last_status;
}

// Main execution function - fork and execute all commands
void execute_commands(t_execution *exec, t_list ***env)
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
                execute_external(cmd, **env);  // Never returns
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
```

**Key Points:**
- Loop through all commands
- Skip `EXEC_BUILTIN_PARENT` (already executed)
- Fork for `EXEC_BUILTIN_CHILD` and `EXEC_EXTERNAL`
- In child:
  1. Connect pipes first
  2. Apply redirections (overrides pipes)
  3. Close ALL pipe fds
  4. Restore signals (SIGINT, SIGQUIT to SIG_DFL)
  5. Execute builtin or external command
- In parent:
  - Store PID
  - Never call dup2 or execve
  - Continue loop
- After loop:
  - Parent closes all pipes
  - Wait for all children
  - Last command's exit status becomes `$?`

---

## Update `minishell.h`

Add these includes after existing includes:
```c
#include <fcntl.h>      // For open(), O_RDONLY, O_WRONLY, etc.
#include <sys/stat.h>   // For file permissions (0644)
#include <errno.h>      // For errno, EACCES
```

Add these function declarations in the "noor" section (after existing noor functions):
```c
// redirections.c
int apply_redirections(t_redir *redirs);

// path_utils.c
char *find_command_path(char *cmd, t_list *env);

// env_utils.c
char **env_list_to_array(t_list *env);
void free_env_array(char **envp);

// execution.c
void execute_commands(t_execution *exec, t_list ***env);
```

---

## Update `makefile`

Change SRCS line to include new files:
```makefile
SRCS = ./qadah/main.c ./qadah/utliltes1.c ./qadah/signal_handle.c ./qadah/lexer.c ./qadah/lexer1.c ./qadah/lexer2.c ./qadah/lexer3.c \
		./qadah/parser_check_syntax.c ./qadah/parser_redirections.c ./qadah/parser_utliltes.c \
		./qadah/parser_utliltes2.c ./qadah/parser.c \
		./noor/builtin_execution.c \
		./noor/builtins.c \
		./noor/fill.c \
		./noor/pipe_handling.c \
		./noor/preparation.c \
		./noor/preparation_utils.c \
		./noor/redirections.c \
		./noor/path_utils.c \
		./noor/env_utils.c \
		./noor/execution.c \
```

---

## How to Integrate into main.c

In `qadah/main.c`, in the `stages()` function, after expander, add:

```c
// After expander(cmds, envc);
t_execution *exec;

// Stage 1-3: Preparation
exec = preparation(cmds);
if (!exec)
{
    cmds_cleaner(cmds);
    return (1);
}

// Execute parent builtins first
execute_builtins_parent(exec, &envc);

// Stage 4: Fork and execute
execute_commands(exec, &envc);

// Cleanup
free_execs(exec);
```

---

## Testing Checklist

### 1. Single Commands
```bash
ls
ls -l
/bin/ls
./minishell
pwd
echo hello
```

### 2. Redirections
```bash
echo hello > file
cat < file
echo test >> file
cat file1 > file2
ls > out 2> err  # (error redirection not required for mandatory)
```

### 3. Pipes
```bash
ls | wc -l
echo hello | cat
cat file | grep test | wc
ls | cat | cat
```

### 4. Builtins
```bash
cd /tmp
pwd
export TEST=value
env | grep TEST
unset TEST
exit 42
```

### 5. Builtins in Pipelines
```bash
echo hello | cat        # echo runs in child
cd /tmp | cat           # cd runs in child (doesn't affect parent)
export A=1 | cat        # export runs in child (doesn't affect parent)
```

### 6. Exit Status
```bash
ls
echo $?                 # Should be 0
ls /nonexistent
echo $?                 # Should be non-zero
true | false
echo $?                 # Should be 1 (last command)
```

### 7. Error Cases
```bash
/nonexistent/command    # Should print "command not found" and exit 127
cat < nonexistent       # Should print error and exit 1
ls | /bad/cmd | cat     # Should handle fork/exec errors gracefully
```

---

## Important Notes

### Exit Codes
- `0` - Success
- `1` - General error
- `126` - Permission denied (command found but not executable)
- `127` - Command not found
- `128 + N` - Killed by signal N

### Process Management
- Parent NEVER calls `dup2()` or modifies its stdin/stdout
- Parent NEVER calls `execve()`
- All pipe fds must be closed in child after `dup2()`
- All pipe fds must be closed in parent after fork loop
- Must wait for ALL children before returning

### Signal Handling
- Parent has custom signal handlers (already implemented)
- Child must restore default: `signal(SIGINT, SIG_DFL)`
- Child must restore default: `signal(SIGQUIT, SIG_DFL)`

### Memory Management
- Free `path` from `find_command_path()`
- Free `envp` from `env_list_to_array()`
- Close file descriptors after use
- Free execution context with `free_execs()`

### Builtins Behavior
- `cd`, `export`, `unset`, `exit` in parent → affect shell state
- Same builtins in pipeline → run in child, don't affect parent
- `echo`, `pwd`, `env` → safe in both parent and child

---

## Common Mistakes to Avoid

❌ **DON'T** call `dup2()` in parent process  
❌ **DON'T** close stdin/stdout (0, 1) directly  
❌ **DON'T** wait for children before closing parent's pipes  
❌ **DON'T** forget to close pipe fds in child after `dup2()`  
❌ **DON'T** forget to restore signals in child  
❌ **DON'T** use last builtin's exit status if it ran in parent  
❌ **DON'T** free environment list (it's shared)  

✅ **DO** close all pipe fds in child after `dup2()`  
✅ **DO** close all pipe fds in parent after fork loop  
✅ **DO** wait for children AFTER parent closes pipes  
✅ **DO** restore signals to SIG_DFL in child  
✅ **DO** update `g_exit_status` with last command's exit  
✅ **DO** handle fork/execve failures gracefully  

---

## Implementation Order

1. ✅ Create `noor/redirections.c`
2. ✅ Create `noor/path_utils.c`
3. ✅ Create `noor/env_utils.c`
4. ✅ Create `noor/execution.c`
5. ✅ Update `minishell.h` (includes + declarations)
6. ✅ Update `makefile` (add new .c files)
7. ✅ Compile: `make`
8. ✅ Integrate into `main.c`
9. ✅ Test each feature separately
10. ✅ Test combined features
11. ✅ Test error cases

---

## Expected Behavior

### Example 1: Simple Command
```bash
$ ls -l
# Should execute /bin/ls with -l argument
# Exit status: 0
```

### Example 2: Pipeline
```bash
$ echo hello | wc -l
# fork child1: execute echo, stdout → pipe[0]
# fork child2: stdin ← pipe[0], execute wc
# Output: 1
# Exit status: 0 (from wc)
```

### Example 3: Redirection
```bash
$ echo test > file
# fork child: execute echo
# Redirect stdout to file
# Exit status: 0
```

### Example 4: Builtin in Parent
```bash
$ cd /tmp
# NO fork - execute in parent
# Current directory changes
# Exit status: 0
```

### Example 5: Builtin in Pipeline
```bash
$ echo hello | cd /tmp | cat
# fork child1: execute echo
# fork child2: execute cd (in child, doesn't affect parent)
# fork child3: execute cat
# Parent directory UNCHANGED
```

---

## Debugging Tips

1. **Print PIDs:** Add debug prints to see which process is which
2. **Check file descriptors:** Use `lsof -p <pid>` to see open fds
3. **Check signals:** Verify child has default signal handlers
4. **Check exit status:** Print `$?` after each command
5. **Use strace:** `strace -f ./minishell` to see system calls
6. **Check leaks:** `valgrind --leak-check=full ./minishell`

---

## Resources

- `man fork` - Process creation
- `man execve` - Execute program
- `man waitpid` - Wait for process state change
- `man dup2` - Duplicate file descriptor
- `man pipe` - Create pipe
- `man access` - Check file accessibility
- `man open` - Open file
- `man signal` - ANSI C signal handling

---

## Final Checklist

Before considering Stage 4 complete:

- [ ] All 4 files created (redirections, path_utils, env_utils, execution)
- [ ] minishell.h updated with includes and declarations
- [ ] makefile updated with new source files
- [ ] Code compiles without warnings
- [ ] Single commands work (ls, pwd, echo)
- [ ] Redirections work (<, >, >>)
- [ ] Pipes work (cmd1 | cmd2)
- [ ] Builtins work in parent
- [ ] Builtins work in pipelines (run in child)
- [ ] Exit status propagates correctly
- [ ] PATH resolution works
- [ ] Absolute/relative paths work
- [ ] Error messages displayed correctly
- [ ] Fork failures handled
- [ ] Execve failures handled (127, 126)
- [ ] No memory leaks (valgrind)
- [ ] No file descriptor leaks
- [ ] Parent stdin/stdout never modified
- [ ] Signals work correctly (Ctrl+C, Ctrl+\)

---

## Good Luck! 🚀

This is the most complex stage, but you have all the foundation ready. Take it step by step, test each component individually, and you'll have a working shell!

Remember: The parent process is sacred - never modify its stdin/stdout/stderr!
