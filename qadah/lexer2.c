/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 17:24:28 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*allocate_word_size_double_quoted(char *line, int *i, t_token **tokens)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	len = 0;
	if (line[*i] == '"')
	{
		(*i)++;
		while (line[*i] && line[*i] != '"')
			(*i)++;
		if (line[*i] == '"')
			(*i)++;
	}
	len = *i - start;
	*i = start;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		tokens_mem_allocation_failed(tokens);
	return (word);
}

char	*allocate_word_size_single_quoted(char *line, int *i, t_token **tokens)
{
	int		start;
	int		len;
	char	*word;

	start = *i;
	len = 0;
	if (line[*i] == '\'')
	{
		(*i)++;
		while (line[*i] && line[*i] != '\'')
			(*i)++;
		if (line[*i] == '\'')
			(*i)++;
	}
	len = *i - start;
	*i = start;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		tokens_mem_allocation_failed(tokens);
	return (word);
}

void	handle_single_quoted_words(char *line, int *i, t_token **tokens,
		t_token *node)
{
	int		len;
	char	*word;

	len = 0;
	word = allocate_word_size_single_quoted(line, i, tokens);
	if (line[*i] == '\'')
	{
		(*i)++;
		while (line[*i] && line[*i] != '\'')
		{
			word[len] = line[*i];
			(*i)++;
			len++;
		}
		if (line[*i] == '\'')
			(*i)++;
		word[len] = '\0';
		node->next = NULL;
		node->quote_type = 2;
		node->value = word;
		node->type = TOKEN_WORD;
		tokens_add_back(tokens, node);
		return ;
	}
}

void	word_assign(t_token *node, char *word, int len)
{
	word[len] = '\0';
	node->next = NULL;
	node->quote_type = 0;
	node->value = word;
	node->type = TOKEN_WORD;
}

void	word_assign_double_quoted(t_token *node, char *word, int len)
{
	word[len] = '\0';
	node->next = NULL;
	node->quote_type = 1;
	node->value = word;
	node->type = TOKEN_WORD;
}
