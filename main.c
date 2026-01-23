#include "minishell.h"

int g_exit_status = 0;


// typedef struct s_redir
// {
//     t_token_type    type;
//     char            *file;
//     int         quote_type;
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


//for printing the parser
/*
void print_redirs(t_redir *redir)
{
    while (redir)
    {
        char *type_str;
        char *type_name;
        if (redir->type == TOKEN_REDIR_IN)
        {
            type_str = "<";
            type_name = "REDIR_IN";
        }
        else if (redir->type == TOKEN_REDIR_OUT)
        {
            type_str = ">";
            type_name = "REDIR_OUT";
        }
        else if (redir->type == TOKEN_REDIR_APPEND)
        {
            type_str = ">>";
            type_name = "REDIR_APPEND";
        }
        else if (redir->type == TOKEN_HEREDOC)
        {
            type_str = "<<";
            type_name = "HEREDOC";
        }
        else
        {
            type_str = "?";
            type_name = "UNKNOWN";
        }

        printf("    Redir: %s (%s) -> %s (expandable: %d)\n", type_str, type_name, redir->file, redir->quote_type);
        redir = redir->next;
    }
}

void print_parser(t_cmd *cmds)
{
    int cmd_num = 1;
    while (cmds)
    {
        printf("Command %d:\n", cmd_num);
        printf("  pipe_in: %d, pipe_out: %d\n", cmds->pipe_in, cmds->pipe_out);

        // print argv
        printf("  argv: ");
        for (int i = 0; cmds->argv && cmds->argv[i]; i++)
        {
            printf("\"%s\" ", cmds->argv[i]);
        }
        printf("\n");

        // print quote_types
        printf("  quote_types: ");
        for (int i = 0; cmds->quote_types && cmds->argv && cmds->argv[i]; i++)
        {
            printf("%d ", cmds->quote_types[i]);
        }
        printf("\n");

        // print redirections
        if (cmds->redirs)
            print_redirs(cmds->redirs);

        printf("\n");
        cmds = cmds->next;
        cmd_num++;
    }
}

*/


void expander(t_cmd *cmds)
{
    int i;
    t_cmd *tmp;
    t_redir *r;

    tmp = cmds;
    while (tmp)
    {
        i = 0;
        while (tmp->argv[i])
        {
            if (tmp->quote_types[i] == 1 && tmp->argv[i][0] == '$')
            {
                // expand
            }
            i++;
        }
        r = tmp->redirs;
        while (r)
        {
            if (r->quote_type == 1 && r->file[0] == '$')
            {
                // expand
            }
            r = r->next;
        }
        tmp = tmp->next;
    }
}


int stages(char *line,char **env)
{
    t_token *tokens;
    t_list  *envc;
    t_cmd *cmds;
   

    tokens = tokenizer(line);
    envc = env_copy(env);//copy for the env as a linked list

    cmds = parser(tokens);
    if(!cmds)
    {
        tokens_clear(&tokens, free);
        cmds_cleaner(cmds);
        return(1);
    }
    //print_parser(cmds);
    tokens_clear(&tokens, free);
    envc = env_copy(env);//copy for the env as a linked list
    //expander
    //excute
    cmds_cleaner(cmds);


    return (0);
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
        if (!*line)
            continue ;
        if(*line && not_all_space(line))//for adding to history
            add_history(line);
        if(check_quotes(line))
        {
            write(2,"Syntax error: unclosed quotes\n",31);
            free(line);
            continue;
        }
        if (stages(line,env))
        {
            free(line);
            continue;
        }
        //printf("%s\n",line);
        free(line);
    }
}
