/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 12:05:32 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/03 18:56:22 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int redirection_in(t_redir *redirs)
{
    int fd;

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
    return (0);
}

int redirection_out(t_redir *redirs)
{
    int fd;

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
    return (0);
}

int redirection_append(t_redir *redirs)
{
    int fd;

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
    return (0);
}

// Signal handler for heredoc in child - exit on SIGINT
static void heredoc_sigint_handler(int sig)
{
    if (sig == SIGINT)
        exit(1);
}

// Child process: read heredoc content and write to pipe
static void heredoc_child(t_redir *redir, int write_fd, t_list *env, t_cmd *cmd)
{
    char *line;
    char *expanded_line;

    // Setup signals in child
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, heredoc_sigint_handler);
    
    while (1)
    {
        line = readline("heredoc> ");
        if (!line)
        {
            write(2, "minishell: warning: here-document delimited by ", 47);
            write(2, "end-of-file (wanted `", 21);
            write(2, redir->file, ft_strlen(redir->file));
            write(2, "')\n", 3);
            break;
        }
        
        // Check if we hit the delimiter
        if (ft_strcmp(line, redir->file) == 0)
        {
            free(line);
            break;
        }
        // احكيلي اقلك عن هاي
        if (redir->quote_type == 0)
        {
            expanded_line = expand_one_arg(line, env, cmd);
            ft_putstr_fd(expanded_line, write_fd);
            ft_putchar_fd('\n', write_fd);
            free(expanded_line);
            free(line);
        }
        else
        {
            ft_putstr_fd(line, write_fd);
            ft_putchar_fd('\n', write_fd);
            free(line);
        }
    }
    
    close(write_fd);
    exit(0);
}

// Read heredoc content from stdin and store in a pipe
// Forks a child to handle the reading (for clean signal handling)
static int read_heredoc_content(t_redir *redir, t_list *env, t_cmd *cmd)
{
    int fd[2];
    pid_t pid;
    int status;

    if (pipe(fd) == -1)
    {
        perror("minishell");
        return (-1);
    }
    
    pid = fork();
    if (pid == -1)
    {
        perror("minishell");
        close(fd[0]);
        close(fd[1]);
        return (-1);
    }
    
    if (pid == 0)
    {
        // Child process
        close(fd[0]);
        heredoc_child(redir, fd[1], env, cmd);
    }
    
    // Parent process
    close(fd[1]);
    waitpid(pid, &status, 0);
    
    // Check if child was interrupted by SIGINT
    if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
    {
        close(fd[0]);
        return (-1);
    }
    
    return (fd[0]);
}

// Process all heredocs for all commands BEFORE execution
// This must be called before any forking happens
int process_heredocs(t_cmd *cmd_list, t_list *env)
{
    t_cmd *cmd;
    t_redir *redir;

    cmd = cmd_list;
    while (cmd)
    {
        redir = cmd->redirs;
        while (redir)
        {
            if (redir->type == TOKEN_HEREDOC)
            {
                redir->heredoc_fd = read_heredoc_content(redir, env, cmd);
                if (redir->heredoc_fd == -1)
                    return (-1);
            }
            else
                redir->heredoc_fd = -1; // Not a heredoc
            redir = redir->next;
        }
        cmd = cmd->next;
    }
    return (0);
}

int redirection_heredoc(t_redir *redirs)
{
    // Heredoc was already read in process_heredocs()
    // Just redirect the stored fd to stdin
    if (redirs->heredoc_fd == -1)
    {
        ft_putstr_fd("minishell: heredoc fd not initialized\n", 2);
        return (-1);
    }
    if (dup2(redirs->heredoc_fd, STDIN_FILENO) == -1)
    {
        perror("minishell");
        return (-1);
    }
    close(redirs->heredoc_fd);
    return (0);
}


int apply_redirections(t_redir *redirs)
{
    while (redirs)
    {
        if (redirs->type == TOKEN_REDIR_IN) // <
        {
            if (redirection_in(redirs) != 0)
                return (-1);
        }
        else if (redirs->type == TOKEN_REDIR_OUT) // >
        {
            if (redirection_out(redirs) != 0)
                return (-1);
        }
        else if (redirs->type == TOKEN_REDIR_APPEND) // >>
        {
            if (redirection_append(redirs) != 0)
                return (-1);
        }
        // TOKEN_HEREDOC:
        else if (redirs->type == TOKEN_HEREDOC)
        {
            if (redirection_heredoc(redirs) != 0)
                return (-1);
        }
        redirs = redirs->next;
    }
    return (0);
}