# 🎯 MINISHELL - 42 PROJECT CHECKLIST (MANDATORY PART ONLY)

## ⚠️ CRITICAL ISSUES (Must Fix Before Testing)

### 1. ❌ HEREDOC NOT IMPLEMENTED
**Status**: Token recognized but NOT executed  
**Files**: `noor/redirections.c` line 93  
**What to do**:
- [ ] Implement `redirection_heredoc()` function in `noor/redirections.c`
- [ ] Read from stdin until delimiter is found
- [ ] Handle quotes in delimiter (if quoted, no expansion; if unquoted, expand $VAR)
- [ ] Store in temporary file or pipe
- [ ] Redirect stdin to the heredoc content
- [ ] Close and unlink temporary file after use
```c
// Add to noor/redirections.c:
int redirection_heredoc(t_redir *redirs)
{
    // TODO: Implement heredoc
    // 1. Create temp file or pipe
    // 2. Read lines until delimiter
    // 3. Expand variables if delimiter not quoted
    // 4. Redirect stdin to temp file
    // 5. Clean up temp file
}
```
- [ ] Update `apply_redirections()` to call `redirection_heredoc()` instead of skipping

---

## 🔴 HIGH PRIORITY ISSUES

### 2. ⚠️ SIGNAL HANDLING IN CHILD PROCESSES
**Status**: Only parent handles signals  
**Files**: `noor/execution.c` lines 152-153  
**Current**: Signals restored to SIG_DFL in child  
**Problem**: Ctrl+C during heredoc or command execution might not work correctly

**What to do**:
- [ ] Test signal handling during:
  - [ ] Heredoc input (Ctrl+C should cancel heredoc)
  - [ ] Command execution (Ctrl+C should kill command, not shell)
  - [ ] Ctrl+\ (SIGQUIT) should be ignored in parent, kill in child
- [ ] Update signal handling if tests fail

### 3. ⚠️ FILE DESCRIPTOR LEAKS
**Status**: Needs verification  
**Files**: `noor/redirections.c`, `noor/pipe_handling.c`, `noor/execution.c`

**What to do**:
- [ ] Check ALL file descriptors are closed in error cases:
  - [ ] `redirection_in()` - fd closed on dup2 error ✅
  - [ ] `redirection_out()` - fd closed on dup2 error ✅
  - [ ] `redirection_append()` - fd closed on dup2 error ✅
  - [ ] `redirection_heredoc()` - NOT IMPLEMENTED ❌
- [ ] Check pipes are closed in ALL paths:
  - [ ] Parent after fork loop ✅
  - [ ] Child after dup2 ✅
  - [ ] On fork failure - MISSING ❌
- [ ] Add cleanup on fork failure in `execution.c` line 128

**Fix needed**:
```c
// In execution.c after fork failure:
if (pid == -1)
{
    perror("minishell: fork");
    exec->pids[i] = -1;
    // ADD: Close pipes if this was the last command
    if (i == exec->cmd_count - 1 && exec->pipes)
        close_all_pipes(exec->pipes, exec->cmd_count - 1);
    cmd = cmd->next;
    i++;
    continue;
}
```

### 4. ⚠️ MEMORY LEAKS
**Status**: Needs testing with valgrind  
**Files**: All noor/*.c files

**What to do**:
- [ ] Test for leaks: `valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./minishell`
- [ ] Check memory freed in:
  - [ ] Normal execution path ✅ (free_execs called)
  - [ ] Error paths - NEEDS REVIEW ⚠️
  - [ ] Signal interruption (Ctrl+C) - NOT HANDLED ❌
  - [ ] Parsing errors - QADAH'S RESPONSIBILITY
- [ ] Add signal handler to free resources on SIGINT during execution

### 5. ⚠️ EXPORT WITHOUT ARGUMENTS
**Status**: Not implemented  
**Files**: `noor/builtins.c` - `builtin_export()`  
**Current**: Returns 0 if no arguments  
**Expected**: Should print all exported variables in format: `declare -x VAR="value"`

**What to do**:
- [ ] Update `builtin_export()` to print all env when no args:
```c
// In builtin_export():
if (!argv[1])
{
    // Print all env variables in "declare -x" format
    t_list *cur = *env;
    while (cur)
    {
        printf("declare -x %s\n", (char *)cur->content);
        cur = cur->next;
    }
    return (0);
}
```

---

## 🟡 MEDIUM PRIORITY ISSUES

### 6. ⚠️ EXIT STATUS HANDLING
**Status**: Partially implemented  
**Files**: `noor/execution.c`, `noor/execution_wrapper.c`  
**Issues**:
- Exit status updated in `wait_all_children()` ✅
- Exit status from builtins not properly propagated ⚠️

**What to do**:
- [ ] Verify exit status is correct for:
  - [ ] Successful commands (should be 0)
  - [ ] Failed commands (should be command's exit code)
  - [ ] Command not found (should be 127) ✅
  - [ ] Permission denied (should be 126) ✅
  - [ ] Signal termination (should be 128 + signal number) ✅
  - [ ] Builtins (cd, export, unset return values)
- [ ] Test: `echo $?` after each command type

### 7. ⚠️ CD ERROR MESSAGES
**Status**: Incorrect error format  
**Files**: `noor/builtins.c` - `builtin_cd()`

**What to do**:
- [ ] Update error messages to match bash format:
```c
// Current: perror("cd")
// Should be: 
ft_putstr_fd("minishell: cd: ", 2);
ft_putstr_fd(path, 2);
ft_putstr_fd(": No such file or directory\n", 2);
```

### 8. ⚠️ PWD ENVIRONMENT VARIABLE
**Status**: Not properly updated  
**Files**: `noor/builtins.c` - `builtin_cd()`  
**Issue**: CD updates OLDPWD but not PWD consistently

**What to do**:
- [ ] Verify PWD and OLDPWD are updated after each `cd` command
- [ ] Test with: `cd /tmp && pwd && echo $PWD`

### 9. ⚠️ ECHO -n FLAG HANDLING
**Status**: Basic implementation  
**Files**: `noor/builtins.c` - `builtin_echo()`  
**Issue**: Only checks exact `-n`, should handle `-nnn`, `-nnnnnn` etc.

**What to do**:
- [ ] Update to handle multiple n's: `-nnnnn` should work like `-n`
```c
// Check if flag is -n with any number of n's
int is_n_flag(char *str)
{
    int i = 0;
    if (str[i++] != '-')
        return (0);
    if (str[i] != 'n')
        return (0);
    while (str[i] == 'n')
        i++;
    return (str[i] == '\0');
}
```

### 10. ⚠️ UNSET VALIDATION
**Status**: Missing validation  
**Files**: `noor/builtins.c` - `builtin_unset()`  
**Issue**: Doesn't validate variable names

**What to do**:
- [ ] Add validation: variable name must start with letter/underscore
- [ ] Can only contain alphanumeric and underscore
- [ ] Print error for invalid names

### 11. ⚠️ EXIT NUMERIC OVERFLOW
**Status**: Basic check only  
**Files**: `noor/builtins.c` - `builtin_exit()`  
**Issue**: `ft_atoi(argv[1]) < 0` doesn't handle overflow properly

**What to do**:
- [ ] Use `ft_atol()` instead of `ft_atoi()`
- [ ] Check for long long overflow
- [ ] Handle values > LLONG_MAX

---

## 📋 TESTING CHECKLIST

### Mandatory Features Test

**Builtins:**
- [ ] `echo` with and without `-n`
- [ ] `echo -n -n -n test` (multiple -n flags)
- [ ] `cd` with relative and absolute paths
- [ ] `cd` without arguments (should go to HOME)
- [ ] `cd` with invalid path
- [ ] `pwd` from different directories
- [ ] `export` without arguments (list all)
- [ ] `export VAR=value`
- [ ] `export VAR="value with spaces"`
- [ ] `unset VAR`
- [ ] `env` (list environment)
- [ ] `exit` without arguments
- [ ] `exit 42`
- [ ] `exit abc` (error: numeric argument required)
- [ ] `exit 1 2` (error: too many arguments)

**Redirections:**
- [ ] `echo hello > file`
- [ ] `echo world >> file`
- [ ] `cat < file`
- [ ] `cat << EOF` (heredoc) ❌ NOT IMPLEMENTED
- [ ] Multiple redirections: `< in1 < in2 > out1 > out2`
- [ ] Redirect to non-existent directory: `echo test > /nonexist/file`

**Pipes:**
- [ ] Simple pipe: `echo hello | cat`
- [ ] Multiple pipes: `ls | grep test | wc -l`
- [ ] Pipe with redirections: `cat < file | grep word > output`

**Environment Variables:**
- [ ] `echo $HOME`
- [ ] `echo $USER`
- [ ] `echo $?` (exit status)
- [ ] `echo $NONEXISTENT` (should print empty)
- [ ] `export TEST=123 && echo $TEST`

**Quotes:**
- [ ] `echo "hello world"`
- [ ] `echo 'hello world'`
- [ ] `echo "$HOME"` (should expand)
- [ ] `echo '$HOME'` (should NOT expand)
- [ ] Mixed quotes: `echo "Hello '$USER'"`

**Signals:**
- [ ] Ctrl+C in prompt (new line)
- [ ] Ctrl+C during command execution (kill command)
- [ ] Ctrl+\ in prompt (ignored)
- [ ] Ctrl+\ during command execution (kill command with core dump)
- [ ] Ctrl+D (exit shell)

**Edge Cases:**
- [ ] Empty command (just spaces)
- [ ] Very long command line
- [ ] Command not found: `nonexistentcommand`
- [ ] Permission denied: `./file_without_x`
- [ ] Multiple spaces: `echo    hello    world`
- [ ] Tabs: `echo	hello	world`

### Memory & FD Test
- [ ] `valgrind --leak-check=full ./minishell` (no leaks)
- [ ] `valgrind --track-fds=yes ./minishell` (all fds closed)
- [ ] Test with many pipes (stress test)
- [ ] Test with many redirections (stress test)

---

## 📝 FILES THAT NEED IMMEDIATE ATTENTION

### Priority 1 (MUST FIX):
1. **noor/redirections.c** - Implement heredoc

### Priority 2 (SHOULD FIX):
2. **noor/builtins.c** - Fix export without args, echo -n handling
3. **noor/execution.c** - Add FD cleanup on fork failure
4. **noor/builtins.c** - Improve error messages

### Priority 3 (NICE TO HAVE):
5. **noor/builtins.c** - Better numeric validation in exit
6. **noor/builtins.c** - Unset validation
7. Add comprehensive tests

---

## 🚀 RECOMMENDED WORKFLOW

1. **Day 1-2: Fix Norminette**
   - Fix all indentation issues in noor/*.c files
   - Run `norminette` until all files pass

2. **Day 3-4: Implement Heredoc**
   - Write `redirection_heredoc()` function
   - Test basic heredoc functionality
   - Handle heredoc with variable expansion

3. **Day 5: Fix Memory Leaks**
   - Run valgrind tests
   - Fix any reported leaks
   - Ensure all FDs are closed

4. **Day 6: Builtin Improvements**
   - Fix export without arguments
   - Improve echo -n handling
   - Better error messages

4. **Day 5-6: Testing**
   - Run all test cases
   - Compare with bash behavior
   - Fix any discovered bugs

5. **Day 7: Edge Cases & Polish**
   - Test edge cases
   - Handle error conditions
   - Final cleanup

---

## ✅ CURRENT WORKING FEATURES
t variables ($VAR, $?)
- ✅ Quotes (single and double)
- ✅ Tokenizer & Parser (qadah's part)
- ✅ Expander (qadah's part)
- ✅ All 7 builtins (basic functionality)
- ✅ Signal handling (basic)
- ✅ Exit status tracking
- ✅ PATH resolution
- ✅ Fork and pipe handling
2. **Day 3
## ❌ MISSING/BROKEN FEATURES

- ❌ Heredoc (<<) - NOT IMPLEMENTED
- ❌ *Day 4te FD management
- ❌ Memory leak fixes
- ❌ Export without arguments
- ❌ Advanced echo -n handling

---

## 📞 QUESTIONS TO ASK YOUR PEER (QADAH)

1. Are there any memory leaks in tokenizer/parser/expander?
2. Is syntax error handling complete?
3. Are quote types properly tracked for heredoc expansion?
4. Is the parser handling empty commands correctly?
5. Any known issues in qadah's part?

---

Good luck! 🚀
