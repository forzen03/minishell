#include "minishell.h"

void	handle_word(char *line, int *i, t_token **tokens)
{
	int		len;
	char	*word;
	t_token	*node;

	len = 0;
	node = malloc(sizeof(t_token));
	if (!node)
		tokens_memory_allocation_failed(tokens);
	if (is_quoted(line, i))
	{
		handle_quoted_words(line, i, tokens, node);
		return ;
	}
	word = allocate_word_size(line, i, tokens);
	while (line[*i] && line[*i] != '|' && line[*i] != '<' && line[*i] != '>'
		&& line[*i] != ' ' && line[*i] != '"' && line[*i] != '\'')
	{
		word[len] = line[*i];
		(*i)++;
		len++;
	}
	word_assign(node, word, len);
	tokens_add_back(tokens, node);
}

t_token	*tokenizer(char *line)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		skip_space(line, &i);
		if (!line[i])//if empty
			break ;
		if (handle_operators(line, &i, &tokens))// | > >> << <
			continue ;
		if (!line[i])//if empty
			break ;
		handle_word(line, &i, &tokens);//every thing else
	}
	return (tokens);
}

int	check_quotes(char *line)//to check the unclosed quotes and handle the inside quotes
{
	int	i;
	int	in_single;
	int	in_double;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (line[i])
	{
		if (line[i] == '"' && !in_single)
			in_double = !in_double;
		else if (line[i] == '\'' && !in_double)
			in_single = !in_single;
		i++;
	}
	if (in_single == 1 || in_double == 1)
		return (1);
	return (0);
}

void	handle_quoted_words(char *line, int *i, t_token **tokens, t_token *node)
{
	int		len;
	char	*word;

	len = 0;
	//word = allocate_word_size_double_quoted(line,i,tokens);
	if (line[*i] == '"')
	{
		word = allocate_word_size_double_quoted(line, i, tokens);
		(*i)++;
		while (line[*i] && line[*i] != '"')
		{
			word[len] = line[*i];
			(*i)++;
			len++;
		}
		if (line[*i] == '"')
			(*i)++;
		word_assign_double_quoted(node, word, len);
		tokens_add_back(tokens, node);
		return ;
	}
	else if (line[*i] == '\'')
		handle_single_quoted_words(line, i, tokens, node);
}