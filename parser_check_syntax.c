#include "minishell.h"

int check_syntax_error(t_token **token)
{
	t_token *last_tmp;
	t_token *tmp;

	tmp = *token;

	if (!tmp)
		return (0);
	last_tmp = *token;
    // Check if first token is a pipe

	if (tmp->type == TOKEN_PIPE)
	{
		write(2, "Syntax error near unexpected token `|'\n", 40);
		g_exit_status = 2;
		return (1);
	}

    // Check for consecutive operators
	if (check_consecutive_operators(tmp))
		return (1);

    // Check if last token is not a word
	if (check_last_operators(last_tmp))
		return (1);

	return (0);

}

int check_last_operators(t_token *last_tmp)
{
	last_tmp = ft_lstlast_tokens(last_tmp);

	if (last_tmp->type != TOKEN_WORD)
	{

		if (last_tmp->type == TOKEN_PIPE)
			 write(2, "Syntax error near unexpected token `|'\n", 40);
		else
			write(2, "Syntax error near unexpected token `newline'\n", 46);
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

int check_consecutive_redirections_and_pipe(t_token *tmp)
{

	if (tmp->next->type != TOKEN_WORD)
	{

		if (tmp->next->type == TOKEN_PIPE)
			write(2, "Syntax error near unexpected token `|'\n", 40);

		else if (tmp->next->type == TOKEN_HEREDOC)
			write(2, "Syntax error near unexpected token `<<'\n", 41);

		else if (tmp->next->type == TOKEN_REDIR_APPEND)
				write(2, "Syntax error near unexpected token `>>'\n", 41);

		else if (tmp->next->type == TOKEN_REDIR_IN)
				write(2, "Syntax error near unexpected token `<'\n", 40);

		else if (tmp->next->type == TOKEN_REDIR_OUT)
				write(2, "Syntax error near unexpected token `>'\n", 40);
		g_exit_status = 2;
		return (1);
	}
	return (0);
}

int check_consecutive_operators(t_token *tmp)
{

	while (tmp)
	{
        // Redirections must be followed by a filename (word)
		if (is_redirection(tmp->type) && tmp->next)
		{
			if (check_consecutive_redirections_and_pipe(tmp))
				return (1);
		}
        // Pipe followed by pipe is syntax error 

		else if (tmp->type == TOKEN_PIPE && tmp->next)
		{

			if (tmp->next->type == TOKEN_PIPE)
			{
				write(2, "Syntax error near unexpected token `|'\n", 40);
				g_exit_status = 2;
				return (1);
			}
		}
		tmp = tmp->next;
	}
	return (0);
}
