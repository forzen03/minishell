# STAGE 4: FORK & EXECUTION - Implementation Instructions

## What This Stage Does
Execute commands by forking child processes, connecting pipes, applying redirections, and running builtins or external commands.

---

## Prerequisites (Already Completed)
✅ Stage 1: `preparation()` - Command counting, pipe allocation, execution type detection  
✅ Stage 2: `connect_pipes_for_child()`, `close_all_pipes()` - Pipe management  
✅ Stage 3: `execute_builtin()`, `execute_builtins_parent()` - Builtin execution  

---

## Files to Create

### 1. `noor/redirections.c`
**Purpose:** Apply file redirections (`<`, `>`, `>>`)

**Logic:**
```
Loop through t_redir linked list:
    If type == TOKEN_REDIR_IN (<):
        fd = open(file, O_RDONLY)
        dup2(fd, STDIN_FILENO)
        close(fd)
    
    If type == TOKEN_REDIR_OUT (>):
        fd = open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644)
        dup2(fd, STDOUT_FILENO)
        close(fd)
    
    If type == TOKEN_REDIR_APPEND (>>):
        fd = open(file, O_CREAT | O_WRONLY | O_APPEND, 0644)
        dup2(fd, STDOUT_FILENO)
        close(fd)
    
    Move to next redirection

Return 0 on success, -1 on error
```

**Key Function:**
- `int apply_redirections(t_redir *redirs)`

**Important:**
- Apply redirections AFTER pipes (so they override)
- Close original fd after dup2
- Return -1 on any error (child will exit(1))

---

### 2. `noor/path_utils.c`
**Purpose:** Find executable in PATH or handle absolute/relative paths

**Logic:**
```
If cmd contains '/':
    If access(cmd, X_OK) succeeds:
        Return strdup(cmd)
    Else:
        Return NULL

Get PATH from environment list:
    Loop through env until find "PATH="
    Extract value after "PATH="

Split PATH by ':' delimiter

For each directory in PATH:
    full_path = directory + "/" + cmd
    If access(full_path, X_OK) succeeds:
        Free paths array
        Return full_path
    Free full_path
    Continue

Not found:
    Free paths array
    Return NULL
```

**Key Function:**
- `char *find_command_path(char *cmd, t_list *env)`

**Important:**
- Must handle absolute paths: `/bin/ls`
- Must handle relative paths: `./minishell`
- Must handle PATH search: `ls`
- Caller must free returned string

---

### 3. `noor/env_utils.c`
**Purpose:** Convert environment list to array for execve

**Logic:**
```
Count environment variables in t_list

Allocate char **array of size (count + 1)

Loop through env list:
    array[i] = strdup(env->content)
    Check allocation
    Move to next

array[count] = NULL  // NULL-terminate

Return array
```

**Key Functions:**
- `char **env_list_to_array(t_list *env)`
- `void free_env_array(char **envp)` - Free array and all strings

**Important:**
- Array must be NULL-terminated
- Each string is a copy (don't modify original env)
- Must have a cleanup function

---

### 4. `noor/execution.c`
**Purpose:** Main fork and execution loop

**Main Function Logic:**
```
execute_commands(exec, env):
    i = 0
    cmd = exec->cmd_list
    
    While cmd exists and i < cmd_count:
        
        // Step 1: Skip parent builtins
        If exec->types[i] == EXEC_BUILTIN_PARENT:
            cmd = next
            i++
            continue
        
        // Step 2: Fork
        pid = fork()
        If fork failed:
            Print error
            Mark pid as -1
            Continue
        
        If pid == 0 (Child):
            // Step 3a: Connect pipes
            If pipes exist:
                connect_pipes_for_child(...)
            
            // Step 3b: Apply redirections (overrides pipes)
            If apply_redirections fails:
                exit(1)
            
            // Step 3c: Close all pipe fds
            If pipes exist:
                close_all_pipes(...)
            
            // Step 3d: Restore signals
            signal(SIGINT, SIG_DFL)
            signal(SIGQUIT, SIG_DFL)
            
            // Step 3e: Execute
            If type == EXEC_BUILTIN_CHILD:
                exit(execute_builtin(...))
            Else:
                execute_external(...)  // Never returns
        
        // Step 4: Parent
        exec->pids[i] = pid
        cmd = next
        i++
    
    // Step 5: Parent closes all pipes
    If pipes exist:
        close_all_pipes(...)
    
    // Step 6: Wait for all children
    wait_all_children(...)
```

**Helper: execute_external()**
```
find_command_path():
    If not found:
        Print "command not found"
        exit(127)

env_list_to_array():
    If failed:
        Free path
        exit(1)

execve(path, argv, envp)

If execve returns (failed):
    Print error
    Free resources
    If errno == EACCES:
        exit(126)
    Else:
        exit(127)
```

**Helper: wait_all_children()**
```
For each command index:
    If pid <= 0:
        Skip (no child or fork failed)
    
    waitpid(pid, &status, 0)
    
    If WIFEXITED(status):
        exit_code = WEXITSTATUS(status)
    Else if WIFSIGNALED(status):
        exit_code = 128 + WTERMSIG(status)

Update exec->last_status with LAST exit code
Update g_exit_status
```

**Key Functions:**
- `void execute_commands(t_execution *exec, t_list ***env)`
- `static void execute_external(t_cmd *cmd, t_list *env)`
- `static void wait_all_children(t_execution *exec)`

---

## Update `minishell.h`

**Add includes:**
```c
#include <fcntl.h>      // For open(), O_RDONLY, etc.
#include <sys/stat.h>   // For file permissions
#include <errno.h>      // For errno, EACCES
```

**Add function declarations:**
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

**Add to SRCS:**
```makefile
./noor/redirections.c \
./noor/path_utils.c \
./noor/env_utils.c \
./noor/execution.c \
```

---

## Integration into `main.c`

**In stages() function, after expander():**
```
expander(cmds, envc);

// Create execution context
exec = preparation(cmds);
if (!exec)
    return error

// Execute parent builtins
execute_builtins_parent(exec, &envc);

// Fork and execute remaining commands
execute_commands(exec, &envc);

// Cleanup
free_execs(exec);
```

---

## Process Flow Visualization

```
Parent Process:
  ├─ preparation() → allocate pipes, determine types
  ├─ execute_builtins_parent() → run cd/export/unset/exit if standalone
  ├─ execute_commands():
  │   ├─ Loop commands:
  │   │   ├─ Skip EXEC_BUILTIN_PARENT
  │   │   ├─ fork() → create child
  │   │   └─ Store child PID
  │   ├─ close_all_pipes() → parent closes pipes
  │   └─ wait_all_children() → collect exit status
  └─ free_execs() → cleanup

Child Process:
  ├─ connect_pipes_for_child() → dup2 stdin/stdout to pipes
  ├─ apply_redirections() → dup2 stdin/stdout to files
  ├─ close_all_pipes() → close all pipe fds
  ├─ signal(SIGINT/SIGQUIT, SIG_DFL) → restore signals
  └─ Execute:
      ├─ If builtin: execute_builtin() → exit(status)
      └─ If external: execute_external() → execve() → never returns
```

---

## Critical Rules

### Parent Process
- ❌ **NEVER** call `dup2()` on stdin/stdout
- ❌ **NEVER** call `execve()`
- ✅ Only store PIDs and wait for children
- ✅ Close all pipes AFTER fork loop
- ✅ Wait for all children AFTER closing pipes

### Child Process
- ✅ Connect pipes FIRST
- ✅ Apply redirections SECOND (overrides pipes)
- ✅ Close ALL pipe fds BEFORE execve
- ✅ Restore signals to SIG_DFL
- ✅ Exit immediately after execution

### Pipe Management
- Create pipes in preparation stage
- Parent: Close after all forks
- Child: Close after dup2
- Never close stdin/stdout (0, 1, 2) directly

### Signal Handling
- Parent: Keep custom handlers (already set)
- Child: Restore `signal(SIGINT, SIG_DFL)` and `signal(SIGQUIT, SIG_DFL)`

---

## Exit Codes

| Code | Meaning |
|------|---------|
| 0 | Success |
| 1 | General error |
| 126 | Permission denied (command not executable) |
| 127 | Command not found |
| 128+N | Killed by signal N |

---

## Testing Scenarios

### 1. Single Commands
- `ls` → PATH search
- `/bin/ls` → Absolute path
- `./minishell` → Relative path
- `pwd`, `echo hello` → Builtins

### 2. Redirections
- `echo test > file` → Create/truncate file
- `cat < file` → Read from file
- `echo more >> file` → Append to file
- `cat < in > out` → Multiple redirections

### 3. Pipes
- `ls | wc -l` → 2 commands
- `cat file | grep test | wc` → 3 commands
- `echo hello | cat | cat | cat` → Multiple pipes

### 4. Builtins Behavior
- `cd /tmp` → Executes in parent, changes directory
- `cd /tmp | cat` → Executes in child, parent directory unchanged
- `export A=1` → Parent env modified
- `export B=2 | cat` → Child env only, parent unchanged

### 5. Exit Status
- `ls && echo $?` → Should print 0
- `ls /bad && echo $?` → Should print non-zero
- `true | false && echo $?` → Should print 1 (last command)

### 6. Error Handling
- `/nonexistent` → "command not found", exit 127
- `cat < nonexist` → Error message, exit 1
- Permission errors → exit 126

---

## Common Mistakes to Avoid

| ❌ Don't | ✅ Do |
|----------|-------|
| Call dup2 in parent | Only in child |
| Close stdin/stdout directly | Close pipe fds only |
| Wait before closing pipes | Close pipes then wait |
| Forget to close pipe fds in child | Close after dup2 |
| Use builtin status from parent | Only use from last command |
| Forget signal restoration | Always restore in child |

---

## Implementation Checklist

### File Creation
- [ ] Create `noor/redirections.c` with `apply_redirections()`
- [ ] Create `noor/path_utils.c` with `find_command_path()`
- [ ] Create `noor/env_utils.c` with `env_list_to_array()` and `free_env_array()`
- [ ] Create `noor/execution.c` with `execute_commands()`, helpers

### Header & Build
- [ ] Add includes to `minishell.h` (fcntl.h, sys/stat.h, errno.h)
- [ ] Add function declarations to `minishell.h`
- [ ] Update `makefile` with new source files
- [ ] Compile with `make` - should have no warnings

### Integration
- [ ] Add execution calls in `stages()` function
- [ ] Call `preparation()` after expander
- [ ] Call `execute_builtins_parent()`
- [ ] Call `execute_commands()`
- [ ] Call `free_execs()`

### Testing
- [ ] Test single commands (ls, pwd, echo)
- [ ] Test absolute/relative paths
- [ ] Test redirections (<, >, >>)
- [ ] Test pipes (2-3 commands)
- [ ] Test builtins standalone
- [ ] Test builtins in pipelines
- [ ] Test exit status propagation
- [ ] Test error cases
- [ ] Check with valgrind (no leaks)
- [ ] Verify parent stdin/stdout unchanged

---

## Debugging Checklist

If something doesn't work:

- [ ] Check if pipes are closed in both parent and child
- [ ] Verify signals restored in child (SIGINT, SIGQUIT)
- [ ] Confirm parent never calls dup2
- [ ] Check PATH environment exists
- [ ] Verify command has execute permission
- [ ] Check file permissions (0644 for output files)
- [ ] Verify redirections applied after pipes
- [ ] Check exit status propagation (last command)
- [ ] Verify no file descriptor leaks (`lsof -p <pid>`)
- [ ] Check memory leaks with valgrind

---

## Final Verification

Before considering Stage 4 complete:

**Functionality:**
- [ ] All commands execute correctly
- [ ] Pipes connect properly (data flows)
- [ ] Redirections work (files created/read)
- [ ] Builtins work (both parent and child contexts)
- [ ] Exit status correct ($?)
- [ ] Error messages displayed properly

**Process Management:**
- [ ] Parent stdin/stdout never modified
- [ ] All children waited for
- [ ] No zombie processes
- [ ] Signals work (Ctrl+C stops child, not parent)

**Resource Management:**
- [ ] No memory leaks (valgrind clean)
- [ ] No file descriptor leaks
- [ ] All pipes closed properly
- [ ] All malloc'd memory freed

---

## Quick Reference

**Order of Operations in Child:**
1. `connect_pipes_for_child()` - dup2 pipes
2. `apply_redirections()` - dup2 files
3. `close_all_pipes()` - close all pipe fds
4. `signal()` - restore SIGINT/SIGQUIT
5. `execute` - builtin or execve

**Order of Operations in Parent:**
1. Loop and fork all children
2. Store all PIDs
3. `close_all_pipes()` - after loop
4. `wait_all_children()` - collect status

**Exit Immediately If:**
- fork() fails → continue to next command
- dup2() fails → child exit(1)
- open() fails → child exit(1)
- command not found → child exit(127)
- execve() fails → child exit(126 or 127)

---

## Summary

This stage ties everything together. You're creating processes, connecting them with pipes, redirecting their I/O, and executing commands. The parent process orchestrates everything but never modifies its own I/O. Children do all the work and exit when done.

**Key Insight:** The parent process is the shell itself - it must remain stable and unchanged. Children are disposable workers that do the job and disappear.

Good luck! 🚀
