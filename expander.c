/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 17:24:28 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *ft_charjoin(char *s, char c)
{
	int i;
	char *new;

	if (!s)
	{
		new = malloc(sizeof(char) * 2);

		if (!new)
			return (NULL);
		new[0] = c;
		new[1] = '\0';
		return (new);
	}
	new = malloc(sizeof(char) * ((int)ft_strlen(s) + 2));

	if (!new)
		return (NULL);
	i = 0;

	while (s[i])
	{
		new[i] = s[i];
		i++;
	}
	new[i] = c;
	new[i + 1] = '\0';
	free(s);
	return (new);
}

void join_ope(char *join, const char *s1, const char *s2)
{
	int i = 0;
	int j = 0;

	while (s1[i])
	{
		join[i] = s1[i];
		i++;
	}

	while (s2[j])
	{
		join[i + j] = s2[j];
		j++;
	}
	join[i + j] = '\0';
}

void memory_allocation_failed_expand(t_cmd *cmds, t_list *envc)
{
	cmds_cleaner(cmds);
	ft_lstclear(&envc, free);
	write(2, "Memory allocation failed\n", 26);
	exit(1);
}

char *expand_one_arg(char *s, t_list *envc, t_cmd *cmds)
{
	int i = 0;
	char *res = NULL;

	while (s[i])
	{

		if (s[i] == '$')
		{
			i++;
			res = expand_dollar(res, s, &i, envc);

			if (!res)
				memory_allocation_failed_expand(cmds, envc);
		}
		else
		{
			res = ft_charjoin(res, s[i]);

			if (!res)
				memory_allocation_failed_expand(cmds, envc);
			i++;
		}
	}

	if (!res)
		res = ft_strdup("");
	return (res);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	int		n;
	int		len;
	char	*join;

	if (s1 == NULL && s2 == NULL)
		return (NULL);

	if (s1 == NULL)
		return (s2);

	if (s2 == NULL)
		return (s1);
	n = ft_strlen(s1);
	len = ft_strlen(s2);
	join = (char *)malloc(sizeof(char) * (n + len + 1));

	if (join == NULL)
		return (NULL);
	join_ope(join, s1, s2);
	free(s1);
	free(s2);
	return (join);
}
