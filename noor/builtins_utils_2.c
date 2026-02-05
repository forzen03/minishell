/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:05:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 17:10:42 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_env(t_list **env, char *key, char *value)
{
	t_list	*cur;
	t_list	*new;
	char	*full_value;

	cur = *env;
	while (cur)
	{
		if (ft_strncmp(cur->content, key, ft_strlen(key)) == 0)
		{
			free(cur->content);
			cur->content = ft_strjoin(key, value);
			return ;
		}
		cur = cur->next;
	}
	full_value = ft_strjoin(key, value);
	new = ft_lstnew(full_value);
	if (new)
		ft_lstadd_back(env, new);
}

int	is_variable(char *s)
{
	int	i;

	i = 0;
	if (s[i] == '=')
		return (0);
	if (!ft_isalpha(s[i]))
		return (0);
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
