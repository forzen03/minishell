#include "minishell.h"

int g_exit_status = 0;

// Forward declarations
char *expand_dollar(char *res, char *s, int *i, t_list *env);
char *ft_charjoin(char *s, char c);
void join_ope(char *join, const char *s1, const char *s2);


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


char *ft_charjoin(char *s, char c)
{
    int i;
    char *new;

    if (!s)
    {
        new = malloc(sizeof(char) * 2);
        if (!new)
            return (NULL);
        new[0] = c;
        new[1] = '\0';
        return (new);
    }
    i = (int)ft_strlen(s);
    new = malloc(sizeof(char) * (i + 2));
    if(!new)
        return (NULL);
    i = 0;
    while(s[i])
    {
        new[i] = s[i];
        i++;
    }
    new[i] = c;
    new[i + 1] = '\0';
    free(s);
    return (new);
}

void join_ope(char *join, const char *s1, const char *s2)
{
    int i = 0;
    int j = 0;
    
    while (s1[i])
    {
        join[i] = s1[i];
        i++;
    }
    while (s2[j])
    {
        join[i + j] = s2[j];
        j++;
    }
    join[i + j] = '\0';
}
void memory_allocation_failed_expand(t_cmd *cmds,t_list *envc)
{
    cmds_cleaner(cmds);
    ft_lstclear(&envc, free);
    write(2,"Memory allocation failed\n",26);
    exit(1);
}
char *expand_one_arg(char *s, t_list *envc,t_cmd *cmds)
{
    int     i = 0;
    char    *res = NULL;

    while (s[i])
    {
        if (s[i] == '$')
        {
            i++;
            res = expand_dollar(res, s, &i, envc);
            if (!res)
                memory_allocation_failed_expand(cmds,envc);
        }
        else
        {
            res = ft_charjoin(res, s[i]);
            if(!res)
                memory_allocation_failed_expand(cmds,envc);
            i++;
        }
    }
    if (!res)
        res = ft_strdup("");
    return (res);
}
char	*ft_strjoin_free(char *s1, char *s2)
{
	int		n;
	int		len;
	char	*join;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 == NULL)
		return (s2);
	if (s2 == NULL)
		return (s1);
	n = ft_strlen(s1);
	len = ft_strlen(s2);
	join = (char *)malloc(sizeof(char) * (n + len + 1));
	if (join == NULL)
		return (NULL);
	join_ope(join, s1, s2);
    free(s1);
    free(s2);
	return (join);
}
char *get_env_value(t_list *env, char *name,int *flag)
{
    char *entry;

    while (env)
    {
        entry = env->content;
        (*flag) = 0;
        if (!ft_strncmp(entry, name, ft_strlen(name)) && entry[ft_strlen(name)] == '=')
            return ft_strdup(entry + ft_strlen(name) + 1);
        env = env->next;
    }
    (*flag) = 1;
    return (NULL);
}
char *expand_exit_status(char *res,int *i,char *s,int *flag)
{
    char *value;

    if (s[*i] == '?')
    {
        value = ft_itoa(g_exit_status);
        if(!value)
        {
            (*flag) = 2;
            return (NULL);
        }
        (*i)++;
        res = ft_strjoin_free(res,value);
        if(!res)
        {
           free(value);
           (*flag) = 2;
           return (NULL); 
        }
        (*flag) = 0;
        return (res);
    }
    (*flag) = 1;
    return (res);
}
char *expand_dollar(char *res, char *s, int *i, t_list *env)
{
    char *name;
    char *value;
    int start;
    int flag;

    flag = 0;
    res = expand_exit_status(res,i,s,&flag);
    if (flag == 0)
        return (res);
    if (flag == 2)
        return (NULL);
    if (!ft_isalpha(s[*i]) && s[*i] != '_')
        return ft_charjoin(res, '$');
    start = *i;
    while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))//to get the env var
        (*i)++;
    name = ft_substr(s, start, *i - start);//to get the env var
    if (!name)
        return (NULL);
    value = get_env_value(env, name,&flag);//to extract it form the envc 
    free(name);
    if (!value && flag == 1)
        return (res);//if not found in env
    return ft_strjoin_free(res, value);
}

void expand_redirection(t_cmd *tmp,t_list *env)
{
    t_redir *r;
    char *new;

    r = tmp->redirs;
    while (r)
    {
        if (r->quote_type != 2 && r->type != TOKEN_HEREDOC)
        {
            new = expand_one_arg(r->file, env, tmp);
            if (!new)
                new = ft_strdup("");
            free(r->file);
            r->file = new;
        }
        r = r->next;
    }
}

void expander(t_cmd *cmds, t_list *env)
{
    int     i;
    t_cmd   *tmp;
    char *new;

    tmp = cmds;
    while (tmp)
    {
        i = 0;
        while (tmp->argv && tmp->argv[i])
        {
            if (tmp->quote_types[i] != 2)
            {
                new = expand_one_arg(tmp->argv[i], env, tmp);
                if (!new)
                    new = ft_strdup("");
                free(tmp->argv[i]);
                tmp->argv[i] = new;
            }
            i++;
        }
        expand_redirection(tmp,env);
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
    if (!tokens)
        return (0);
    envc = env_copy(env);//copy for the env as a linked list

    cmds = parser(tokens);
    if(!cmds)
    {
        tokens_clear(&tokens, free);
        ft_lstclear(&envc, free);
        cmds_cleaner(cmds);
        return(1);
    }
    //print_parser(cmds);
    tokens_clear(&tokens, free);
    // print_env(envc);
    expander(cmds,envc);
    print_parser(cmds);
    //excute Nour's Part يا رب يخلص
    cmds_cleaner(cmds);
    ft_lstclear(&envc, free);


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
