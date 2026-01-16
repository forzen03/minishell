#include "minishell.h"

int g_exit_status = 0;


// typedef struct s_redir
// {
//     t_token_type    type;
//     char            *file;
//     struct s_redir  *next;
// }   t_redir;


// typedef struct s_cmd
// {
//     char            **argv;         // NULL-terminated array
//     int             *quote_types;   // Parallel array (same size)//for expander
//     t_redir         *redirs;
//     int             pipe_in;
//     int             pipe_out;
//     struct s_cmd    *next;
// }   t_cmd;


// void parser(t_token *tokens)
// {
   

// }
void stages(char *line,char **env)
{
    t_token *t;
    t_token *tokens;
    t_list  *envc;

    (void)envc;
    (void)env;
    t = NULL;
    tokens = NULL;
    tokens = tokenizer(line);
    envc = env_copy(env);//copy for the env as a linked list

    t = tokens;
    while(tokens)
    {
        printf("(%d) %s (%d)\n",tokens->type,tokens->value,tokens->quote_type);
        tokens = tokens->next;
    }
    printf("\n");
    tokens_clear(&t,free);


    // parcer//now i shoudld go to the parser stage shall we 

    // expander
    // excution
    // free
}
int main(int argc,char **argv,char **env)
{
    (void)argc;
    (void)argv;
    t_term  term;
    char    *line;

    check_arguments(argc);//check the argc just the ./minishell$ just the excutable
    init_terminal(&term);//for termenal settings
    signal_handle();//for ctrl+c ctrl+/
    while(1)
    {
        line = readline("minishell$ ");
        if(!line)//ctrl+d(EOF)
            exit_handle(&term);
        if(*line && not_all_space(line))//for adding to history
            add_history(line);
        if(check_quotes(line))
        {
            write(2,"Syntax error: unclosed quotes\n",31);
            free(line);
            continue;
        }
        stages(line,env);
        //printf("%s\n",line);
        free(line);
    }
}
