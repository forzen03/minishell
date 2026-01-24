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

// void print_redirs(t_redir *redir)
// {
//     while (redir)
//     {
//         char *type_str;
//         char *type_name;
//         if (redir->type == TOKEN_REDIR_IN)
//         {
//             type_str = "<";
//             type_name = "REDIR_IN";
//         }
//         else if (redir->type == TOKEN_REDIR_OUT)
//         {
//             type_str = ">";
//             type_name = "REDIR_OUT";
//         }
//         else if (redir->type == TOKEN_REDIR_APPEND)
//         {
//             type_str = ">>";
//             type_name = "REDIR_APPEND";
//         }
//         else if (redir->type == TOKEN_HEREDOC)
//         {
//             type_str = "<<";
//             type_name = "HEREDOC";
//         }
//         else
//         {
//             type_str = "?";
//             type_name = "UNKNOWN";
//         }

//         printf("    Redir: %s (%s) -> %s (expandable: %d)\n", type_str, type_name, redir->file, redir->quote_type);
//         redir = redir->next;
//     }
// }

// void print_parser(t_cmd *cmds)
// {
//     int cmd_num = 1;
//     while (cmds)
//     {
//         printf("Command %d:\n", cmd_num);
//         printf("  pipe_in: %d, pipe_out: %d\n", cmds->pipe_in, cmds->pipe_out);

//         // print argv
//         printf("  argv: ");
//         for (int i = 0; cmds->argv && cmds->argv[i]; i++)
//         {
//             printf("\"%s\" ", cmds->argv[i]);
//         }
//         printf("\n");

//         // print quote_types
//         printf("  quote_types: ");
//         for (int i = 0; cmds->quote_types && cmds->argv && cmds->argv[i]; i++)
//         {
//             printf("%d ", cmds->quote_types[i]);
//         }
//         printf("\n");

//         // print redirections
//         if (cmds->redirs)
//             print_redirs(cmds->redirs);

//         printf("\n");
//         cmds = cmds->next;
//         cmd_num++;
//     }
// }

void expand_exit_status(t_cmd *cmds,int i,int j)
{
    int len;
    char *temp;
    int k;
    char *string_1;
    int temp_i;

    temp_i = 0;
    k = 0;
    len = 0;
    while(cmds->argv[i][j] != '$' && cmds->argv[i][j+1] != '?')
    {
        len++;
        j++;
    }
    //make a string
    string_1 = malloc(sizeof(char) * len + 1);
    if (!string_1)
        //free  memory
    len = 0;
    j = 0;
    while(cmds->argv[i][j] != '$' && cmds->argv[i][j+1] != '?')
    {
        string_1[len] = cmds->argv[i][j];
        j++;
    }
    temp = ft_itoa(g_exit_status);
    while(temp[k])
    {
        k++;
        len++;
    }
    
    string_1 =ft_strjoin(string_1,temp);
    if(!string_1)
        //free
    j = j + 2;
    len = 0;
    //make a string
    temp = NULL; 
    temp_i = j;
    while(cmds->argv[i][j])
    {
        len++;
        j++;
    }
    temp = malloc(sizeof(char) * len + 1);
    if(!temp)
        //free;
    j = temp_i;
    len = 0;
     while(cmds->argv[i][j])
    {
        temp[len] = cmds->argv[i][j];
        j++;
    }
    string_1 = ft_strjoin(string_1,temp);
    if(!string_1)
        //free
    cmds->argv[i] = string_1;

}

void expander(t_cmd *cmds)
{
    int i;
    t_cmd *tmp;
    t_redir *r;
    int j;

    j = 0;
    tmp = cmds;
    while (tmp)
    {
        i = 0;
        while (tmp->argv[i])
        {
            j = 0;
            if (tmp->quote_types[i] == 1)//for the argv
            {
                if(tmp->argv[i][j] == '$')
                {
                    if(tmp->argv[i][j + 1] == '?')
                    {
                        //handle the exit status value
                    }
                }
            }
            i++;
        }
        r = tmp->redirs;
        while (r)
        {
            if (r->quote_type == 1)//for the files in redirections
            {
                i = 0;
                while(r->file[i])
                {
                    if(r->file[i] == '$')
                    {

                    }
                    i++;
                }
            }
            r = r->next;
        }
        tmp = tmp->next;
    }
}

// void print_env(t_list *env)
// {
//     t_list *tmp = env;

//     while (tmp)
//     {
//         if (tmp->content) // Only print if value exists
//             printf("%s********\n", (char *)tmp->content);
//         tmp = tmp->next;
//     }
// }


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
    // print_env(envc);
    //expander
    //excute Nour's Part يا رب يخلص
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
