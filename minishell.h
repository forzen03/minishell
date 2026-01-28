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
	int             quote_type;//0=no quotes(expand), 1=double quotes(expand), 2=single quotes(no expand)
	struct s_token *next;
}   t_token;

typedef struct s_redir
{
	t_token_type    type;
	char            *file;
	int             quote_type;
	struct s_redir  *next;
}   t_redir;


typedef struct s_cmd
{
	char            **argv;         // NULL-terminated array
	int             *quote_types;   // Parallel array (same size)//for expander
	t_redir         *redirs;
	int             pipe_in;
	int             pipe_out;
	struct s_cmd    *next;
}   t_cmd;





//prompt && initilization
void readline_loop(t_term term, char **env);//added this here 
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
void word_assign_double_quoted(t_token *node,char *word,int len);
int check_quotes(char *line);
t_token *tokenizer(char *line);

//parser syntax 
int check_syntax_error(t_token **token);
int check_last_operators(t_token *last_tmp);
int check_consecutive_redirections_and_pipe(t_token *tmp);
int check_consecutive_operators(t_token *tmp);

//parser redirections
void add_redir(t_cmd *cmd, t_token **tokens);
void	ft_lstadd_back_redir(t_redir **lst, t_redir *new);
void	ft_lstclear_redirections(t_redir **lst, void (*del)(void *));

//parser utliltes
t_token	*ft_lstlast_tokens(t_token *lst);
void cmds_memory_allocation_failed(t_token *tokens, t_cmd *cmds);
void	ft_lstadd_back_cmds(t_cmd **lst, t_cmd *new);
void intit_parser_node(t_cmd **node);
int argv_len(t_token *tokens);
int	ft_lstsize_cmds(t_cmd *lst);
void cmds_cleaner(t_cmd *cmds);
int is_redirection(t_token_type type);

//main parser functions
t_cmd *parser(t_token *tokens);
void filling_parser_nodes(t_token **tokens, t_cmd **cmds, t_cmd *node);
t_cmd *create_parser_node(t_token *tokens, t_cmd *cmds);

//expander
char *ft_charjoin(char *s, char c);
void join_ope(char *join, const char *s1, const char *s2);
char *ft_charjoin(char *s, char c);
void join_ope(char *join, const char *s1, const char *s2);
void memory_allocation_failed_expand(t_cmd *cmds,t_list *envc);
char *expand_one_arg(char *s, t_list *envc,t_cmd *cmds);
char	*ft_strjoin_free(char *s1, char *s2);
char *get_env_value(t_list *env, char *name,int *flag);
char *expand_exit_status(char *res,int *i,char *s,int *flag);
char *expand_dollar(char *res, char *s, int *i, t_list *env);
void expand_redirection(t_cmd *tmp,t_list *env);
void expander(t_cmd *cmds, t_list *env);
int get_new_len(t_cmd *cmd);
void rebuild_argv(t_cmd *cmd, t_cmd *cmds, t_list *env);
void rebuild_argv_loop(t_cmd *cmd,char    **new_argv,int     *new_quote_types);

#endif