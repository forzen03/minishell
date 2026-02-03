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

char	*ft_charjoin(char *s, char c)
{
	int		i;
	char	*new;

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

void	join_ope(char *join, const char *s1, const char *s2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
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

void	memory_allocation_failed_expand(t_cmd *cmds, t_list *envc)
{
	cmds_cleaner(cmds);
	ft_lstclear(&envc, free);
	write(2, "Memory allocation failed\n", 26);
	exit(1);
}
void check_res_if_NULL(char *res,t_cmd *cmds,t_list *envc)
{
	if (!res)
		memory_allocation_failed_expand(cmds, envc);
}
char	*expand_one_arg(char *s, t_list *envc, t_cmd *cmds)
{
	int		i;
	char	*res;

	i = 0;
	res = NULL;
	while (s[i])
	{
		if (s[i] == '$')
		{
			if (s[i + 1] == '?')
				res = expand_exit_status(res, &i, s, cmds);
			else
				res = expand_dollar(res, s, &i, envc);
			check_res_if_NULL(res,cmds,envc);
		}
		else
		{
			res = ft_charjoin(res, s[i]);
			check_res_if_NULL(res,cmds,envc);
			i++;
		}
	}
	if (!res)
		res = ft_strdup("");
	return (res);
}
