/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer4.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:18:55 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 17:21:39 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_delimiter(char c)
{
	return (c == ' ' || c == '\t' || c == '|' || c == '<' || c == '>');
}

void	skip_quote_section(char *line, int *i, int *len, char quote)
{
	(*i)++;
	while (line[*i] && line[*i] != quote)
	{
		if (len)
			(*len)++;
		(*i)++;
	}
	if (line[*i] == quote)
		(*i)++;
}

int	calculate_word_length(char *line, int *i)
{
	int	start;
	int	len;

	start = *i;
	len = 0;
	while (line[*i] && !is_delimiter(line[*i]))
	{
		if (line[*i] == '"')
			skip_quote_section(line, i, &len, '"');
		else if (line[*i] == '\'')
			skip_quote_section(line, i, &len, '\'');
		else
		{
			len++;
			(*i)++;
		}
	}
	*i = start;
	return (len);
}

void	extract_word_content(char *line, int *i, char *word)
{
	int	len;

	len = 0;
	while (line[*i] && !is_delimiter(line[*i]))
	{
		if (line[*i] == '"')
		{
			(*i)++;
			while (line[*i] && line[*i] != '"')
				word[len++] = line[(*i)++];
			if (line[*i] == '"')
				(*i)++;
		}
		else if (line[*i] == '\'')
		{
			(*i)++;
			while (line[*i] && line[*i] != '\'')
				word[len++] = line[(*i)++];
			if (line[*i] == '\'')
				(*i)++;
		}
		else
			word[len++] = line[(*i)++];
	}
	word[len] = '\0';
}

void	init_for_quotes(int *has_double, int *has_single, int *has_unquotes)
{
	(*has_double) = 0;
	(*has_single) = 0;
	(*has_unquotes) = 0;
}
