/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer3.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 17:24:28 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	detect_quote_type(char *line, int i)
{
	int	has_double;
	int	has_single;
	int	has_unquoted;

	init_for_quotes(&has_double, &has_single, &has_unquoted);
	if (line[i] == '"')
		has_double = 1;
	else if (line[i] == '\'')
		has_single = 1;
	else if (!is_delimiter(line[i]))
		has_unquoted = 1;
	while (line[i] && !is_delimiter(line[i]))
	{
		if (line[i] == '"')
			skip_quote_section(line, &i, NULL, '"');
		else if (line[i] == '\'')
			skip_quote_section(line, &i, NULL, '\'');
		else
		{
			has_unquoted = 1;
			i++;
		}
	}
	return (return_value_of_quotes(has_double, has_single, has_unquoted));
}

void	handle_word(char *line, int *i, t_token **tokens)
{
	t_token	*node;
	char	*word;
	int		len;
	int		quote_type;

	node = malloc(sizeof(t_token));
	if (!node)
		tokens_memory_allocation_failed(tokens);
	quote_type = detect_quote_type(line, *i);
	len = calculate_word_length(line, i);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
	{
		free(node);
		tokens_memory_allocation_failed(tokens);
	}
	extract_word_content(line, i, word);
	word[len] = '\0';
	node->next = NULL;
	node->quote_type = quote_type;
	node->value = word;
	node->type = TOKEN_WORD;
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
		if (!line[i])
			break ;
		if (handle_operators(line, &i, &tokens))
			continue ;
		if (!line[i])
			break ;
		handle_word(line, &i, &tokens);
	}
	return (tokens);
}

int	check_quotes(char *line)
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
