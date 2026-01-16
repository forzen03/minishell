#include "minishell.h"

int g_exit_status = 0;


int main(int argc,char **argv,char **env)
{
    (void)argc;
    (void)argv;
    (void)env;
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

        printf("%s\n",line);
        free(line);
    }
}
