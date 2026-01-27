#include "../minishell.h"


int	ft_lstsize_cmds(t_cmd *lst)
{
	int		i;
	t_cmd	*cur;

	cur = lst;
	i = 0;
	if (cur == NULL)
		return (0);
	while (cur->next != NULL)
	{
		cur = cur->next;
		i++;
	}
	cur = NULL;
	i++;
	return (i);
}

void cmds_cleaner(t_cmd *cmds)
{
    int i;
    t_cmd *tmp;

    while(cmds)
    {
        i = 0;
        if(cmds->quote_types)
            free(cmds->quote_types);
        if (cmds->redirs)
            ft_lstclear_redirections(&cmds->redirs,free);
        if(cmds->argv)
        {
            while (cmds->argv[i])
            {
                free(cmds->argv[i]);
                i++;
            }
            free(cmds->argv);
        }
        tmp = cmds->next;
        free(cmds);
        cmds = tmp;
    }
}

int is_redirection(t_token_type type)
{
    return (type == TOKEN_REDIR_IN    ||
            type == TOKEN_REDIR_OUT   ||
            type == TOKEN_REDIR_APPEND||
            type == TOKEN_HEREDOC);
}
