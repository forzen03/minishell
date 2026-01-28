#include "minishell.h"

void	skip_space(char *line, int *i)
{
	while (line[*i] == ' ' || line[*i] == '\t' || line[*i] == '\n'
		|| line[*i] == '\v' || line[*i] == '\f' || line[*i] == '\r')
	{
		(*i)++;
	}
}

void	tokens_clear(t_token **lst, void (*del)(void *))
{
	t_token	*temp;
	t_token	*next;

	if (!lst || !del)
		return;

	while (*lst)
	{
		temp = *lst;
		next = temp->next;
		del(temp->value);
		free(temp);
		*lst = next;
	}
}

void	tokens_add_back(t_token **lst, t_token *new)
{
	t_token	*cur;

	if (!lst || !new)
		return;

	if (*lst == NULL)
	{
		*lst = new;
		return;
	}
	cur = *lst;

	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

void	tokens_memory_allocation_failed(t_token **tokens)
{
	tokens_clear(tokens, free);
	write(2, "Memory allocation failed\n", 26);
	exit(1);
}

int	check_redirection_in_out(char *line, int *i, t_token *node, t_token **tokens)
{

	if (line[*i] == '<')
	{
		node->type = TOKEN_REDIR_IN;
		node->quote_type = 1;
		node->value = ft_strdup("<");

		if (!node->value)
			tokens_memory_allocation_failed(tokens);
		(*i)++;
		return (1);
	}

	else if (line[*i] == '>')
	{
		node->type = TOKEN_REDIR_OUT;
		node->quote_type = 1;
		node->value = ft_strdup(">");

		if (!node->value)
			tokens_memory_allocation_failed(tokens);
		(*i)++;
		return (1);
	}
	return (0);
}
