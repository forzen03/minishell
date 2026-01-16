/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:42:51 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:42:51 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_finalsize(const char *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && s[i] == c)
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static char	*store(const char *s, int *start, char c)
{
	int		i;
	int		count;
	int		cur;
	char	*d;

	count = 0;
	i = 0;
	cur = *start;
	while (s[cur] != c && s[cur])
	{
		count++;
		cur++;
	}
	d = (char *)malloc((count + 1) * sizeof(char));
	if (d == NULL)
		return (NULL);
	while (s[*start] && s[*start] != c)
	{
		d[i] = s[*start];
		i++;
		(*start)++;
	}
	d[i] = '\0';
	return (d);
}

static void	free_split(char **d, int j)
{
	int	i;

	i = 0;
	while (i < j)
	{
		free(d[i]);
		i++;
	}
	free(d);
}

static	char	**createpointer(const char *s, char c, int *i, int *j)
{
	char	**p;

	if (s == NULL)
		return (NULL);
	p = (char **)malloc((get_finalsize(s, c) + 1) * sizeof(char *));
	if (p == NULL)
		return (NULL);
	*i = 0;
	*j = 0;
	return (p);
}

char	**ft_split(const char *s, char c)
{
	int		i;
	int		j;
	char	**d;

	d = createpointer(s, c, &i, &j);
	if (d == NULL)
		return (NULL);
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i])
		{
			d[j] = store(s, &i, c);
			if (d[j] == NULL)
			{
				free_split(d, get_finalsize(s, c));
				return (NULL);
			}
			j++;
		}
	}
	d[j] = NULL;
	return (d);
}
