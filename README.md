*This project has been created as part of the 42 curriculum by mqadah, njaradat.*

# Minishell

## Description

Minishell is a simplified Unix shell implementation that recreates the basic functionality of bash. This project provides hands-on experience with process creation, file descriptors, signal handling, and the inner workings of a command-line interpreter.

### Key Features

- **Command execution**: Execute commands with absolute or relative paths, as well as commands found in the PATH environment variable
- **Built-in commands**: Implementation of `echo` (with -n option), `cd`, `pwd`, `export`, `unset`, `env`, and `exit`
- **Redirections**: Support for input (`<`), output (`>`), append (`>>`), and heredoc (`<<`)
- **Pipes**: Chain multiple commands together using pipes (`|`)
- **Environment variables**: Expand environment variables (e.g., `$USER`, `$HOME`)
- **Exit status**: Track and expand the exit status of the last executed command (`$?`)
- **Quote handling**: Proper handling of single quotes (`'`) and double quotes (`"`)
- **Signal handling**: Respond to `Ctrl-C`, `Ctrl-D`, and `Ctrl-\` similar to bash behavior

### Project Goal

The goal of this project is to deepen understanding of system programming concepts, including process management, inter-process communication, file descriptor manipulation, and signal handling, while implementing a functional shell that mimics bash behavior in a controlled scope.

## Instructions

### Prerequisites

- **Operating System**: Linux or macOS
- **Compiler**: CC or compatible C compiler
- **Libraries**: readline library must be installed
  - On Ubuntu/Debian: `sudo apt-get install libreadline-dev`
  - On macOS: `brew install readline`

### Compilation

To compile the project, simply run:

```bash
make
```

This will:
1. Compile the libft library
2. Compile all minishell source files
3. Link everything together to create the `minishell` executable

Additional make commands:
- `make clean` - Remove object files
- `make fclean` - Remove object files and the executable
- `make re` - Recompile everything from scratch

### Execution

Run the shell with:

```bash
./minishell
```

You will be greeted with a `minishell$ ` prompt where you can enter commands just like in bash.

### Usage Examples

```bash
minishell$ echo "Hello, World!"
Hello, World!

minishell$ ls -la | grep minishell
-rwxr-xr-x  1 user  staff  123456 Feb  6 2026 minishell

minishell$ export MY_VAR="test"
minishell$ echo $MY_VAR
test

minishell$ cat << EOF > output.txt
heredoc> This is a test
heredoc> EOF
minishell$ cat output.txt
This is a test

minishell$ cd .. && pwd
/Users/user

minishell$ exit
```

## Technical Implementation

### Architecture

The project is organized into several key components:

- **Lexer** (`qadah/lexer*.c`): Tokenizes the input string into words, operators, and special characters
- **Parser** (`qadah/parser*.c`): Builds command structures from tokens, validates syntax, and handles redirections
- **Expander** (`qadah/expander*.c`): Expands environment variables and removes quotes
- **Executor** (`noor/execution*.c`): Creates processes, sets up pipes, and executes commands
- **Built-ins** (`noor/builtin*.c`): Implements shell built-in commands
- **Heredoc** (`noor/heredoc*.c`): Handles heredoc input redirection
- **Signals** (`qadah/signal_handle.c`): Manages signal handling for interactive mode

### Global Variable Usage

The project uses **one global variable** (`g_signal`) to handle signal communication:

- **Purpose**: Signal handlers cannot accept custom parameters, so a global variable is necessary to communicate between the asynchronous signal handler and the main program loop
- **Justification**: This follows the POSIX standard approach for signal handling in C
- **Implementation**: Used to detect `Ctrl-C` (SIGINT) and properly set the exit status to 130

## Resources

### Documentation & References

- [execution + signals](https://www.youtube.com/watch?v=cex9XrZCU14list=PLfqABt5AS4FkW5mOn2Tn9ZZLLDwA3kZUY)

- [minishell concepts] (https://m4nnb3ll.medium.com/minishell-building-a-mini-bash-a-42-project-b55a10598218)

- [Writing Your Own Shell](https://www.cs.purdue.edu/homes/grr/SystemsProgrammingBook/Book/Chapter5-WritingYourOwnShell.pdf) - Tutorial on shell implementation

### AI Usage

AI tools were used during the development of this project for the following purposes:

- **Documentation & Learning**: Understanding complex concepts such as signal handling, file descriptor manipulation, and pipe implementation
- **Debugging Assistance**: Identifying logical errors and suggesting fixes for edge cases
- **Code Review**: Reviewing code structure and suggesting improvements for readability and efficiency
- **Syntax & Best Practices**: Clarifying C programming best practices and POSIX compliance
- **Problem-Solving**: Brainstorming approaches to handle complex scenarios like nested pipes and heredoc handling

**Parts where AI was primarily used:**
- Edge case handling in the parser (e.g., unclosed quotes, invalid redirections)
- Proper implementation of heredoc with signal interruption handling

**Human contribution:**
- Overall architecture and design decisions
- Core implementation of lexer, parser, and executor
- Integration of all components
- Testing, debugging, and validation against bash behavior
- Code organization and team collaboration