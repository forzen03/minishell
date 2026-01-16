#ifndef MINISHELL_H
#define MINISHELL_H

#include "./libft/libft.h"
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <readline/history.h>
#include <termios.h>

extern int g_exit_status;

typedef struct s_term
{
    struct termios original_term;
    struct termios new_term;
} t_term;


typedef enum e_token_type
{
    TOKEN_WORD,//start from 0 
    TOKEN_PIPE,//1
    TOKEN_REDIR_IN,//2
    TOKEN_REDIR_OUT,//3
    TOKEN_HEREDOC,//4
    TOKEN_REDIR_APPEND//5
}   t_token_type;

typedef struct s_token
{
    t_token_type   type;
    char          *value;//
    int             quote_type;//if 1 expandable if 0 its not expandable
    struct s_token *next;
}   t_token;

//prompt && initilization
void env_memory_allocation_fail(t_list *list);
t_list *env_copy(char **env);
int term_setting(t_term *term);
void init_terminal(t_term *term);
void check_arguments(int argc);
void signal_int_handle();
void signal_handle();
void exit_handle(t_term *term);
int not_all_space(char *line);


//tokenizer
void skip_space(char *line,int *i);
void tokens_clear(t_token **lst, void (*del)(void *));
void tokens_add_back(t_token **lst, t_token *new);
void tokens_memory_allocation_failed(t_token **tokens);
int check_redirection_in_out(char *line, int *i, t_token *node,t_token **tokens);
int check_pipe(char *line, int *i, t_token *node,t_token **tokens);
int check_heredoc_and_append(char *line,int *i, t_token *node,t_token **tokens);
int handle_operators(char *line, int *i, t_token **tokens);
char *allocate_word_size(char *line,int *i, t_token **tokens);
int is_quoted(char *line, int *i);
char *allocate_word_size_double_quoted(char *line,int *i,t_token **tokens);
char *allocate_word_size_single_quoted(char *line,int *i,t_token **tokens);
void handle_single_quoted_words(char *line, int *i,t_token **tokens, t_token *node);
void handle_quoted_words(char *line, int *i,t_token **tokens, t_token *node);
void handle_word(char *line,int *i,t_token **tokens);
void word_assign(t_token *node,char *word,int len);
int check_quotes(char *line);
t_token *tokenizer(char *line);

#endif