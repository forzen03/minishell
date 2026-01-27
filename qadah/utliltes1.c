#include "../minishell.h"

void	env_memory_allocation_fail(t_list *list)
{
	ft_lstclear(&list, free);
	write(2, "Memory allocation failed\n", 26);
	exit(1);
}
t_list	*env_copy(char **env)
{
	t_list	*list;
	t_list	*node;
	char	*dup;
	int		i;

	i = 0;
	list = NULL;
	while (env[i])
	{
		dup = ft_strdup(env[i]);
		if (!dup)
			env_memory_allocation_fail(list);
		node = ft_lstnew(dup);
		if (!node)
		{
			free(dup);
			env_memory_allocation_fail(list);
		}
		ft_lstadd_back(&list, node);
		i++;
	}
	return (list);
}

int	term_setting(t_term *term)
{
	if (tcgetattr(STDIN_FILENO, &term->original_term) == -1)
	{
		perror("tcgetattr");
		return (1);
	}
	term->new_term = term->original_term;
	term->new_term.c_lflag = term->new_term.c_lflag & ~(ECHOCTL);
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term->new_term) == -1)
	{
		perror("tcsetattr");
		return (1);
	}
	return (0);
}

void	init_terminal(t_term *term)
{
	if (!isatty(STDIN_FILENO))
	{
		write(2, "minishell: not a terminal\n", 27);
		exit(1);
	}
	if (term_setting(term))
		exit(1);
}
void	check_arguments(int argc)
{
	if (argc != 1)
	{
		write(2, "Error with arguments number\n", 29);
		exit(1);
	}
}