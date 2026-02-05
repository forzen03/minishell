/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 17:24:28 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe(char *line, int *i, t_token *node, t_token **tokens)
{
	if (line[*i] == '|')
	{
		node->type = TOKEN_PIPE;
		node->quote_type = 1;
		node->value = ft_strdup("|");
		if (!node->value)
			tokens_mem_allocation_failed(tokens);
		(*i)++;
		return (1);
	}
	return (0);
}

int	check_heredoc_and_append(char *line, int *i, t_token *node,
		t_token **tokens)
{
	if (line[*i] == '<' && line[*i + 1] == '<')
	{
		node->type = TOKEN_HEREDOC;
		node->quote_type = 1;
		node->value = ft_strdup("<<");
		if (!node->value)
			tokens_mem_allocation_failed(tokens);
		*i += 2;
		return (1);
	}
	else if (line[*i] == '>' && line[*i + 1] == '>')
	{
		node->type = TOKEN_REDIR_APPEND;
		node->quote_type = 1;
		node->value = ft_strdup(">>");
		if (!node->value)
			tokens_mem_allocation_failed(tokens);
		*i += 2;
		return (1);
	}
	return (0);
}

int	handle_operators(char *line, int *i, t_token **tokens)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node)
		tokens_mem_allocation_failed(tokens);
	if (check_heredoc_and_append(line, i, node, tokens)
		|| check_redirection_in_out(line, i, node, tokens) || check_pipe(line,
			i, node, tokens))
	{
		node->next = NULL;
		tokens_add_back(tokens, node);
		return (1);
	}
	free(node);
	return (0);
}

char	*allocate_word_size(char *line, int *i, t_token **tokens)
{
	int		start;
	int		len;
	char	*word;

	len = 0;
	start = *i;
	while (line[*i] && line[*i] != '|' && line[*i] != '<' && line[*i] != '>'
		&& line[*i] != ' ' && line[*i] != '"' && line[*i] != '\'')
	{
		(*i)++;
	}
	len = *i - start;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		tokens_mem_allocation_failed(tokens);
	*i = start;
	return (word);
}

int	is_quoted(char *line, int *i)
{
	if (line[*i] == '"' || line[*i] == '\'')
		return (1);
	return (0);
}
