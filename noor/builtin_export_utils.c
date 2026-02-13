/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 15:42:58 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/13 15:52:23 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_key_cmp(const void *a, const void *b)
{
	t_env_cmp	v;

	v.env_a = *(char **)a;
	v.env_b = *(char **)b;
	v.eq_a = ft_strchr(v.env_a, '=');
	v.eq_b = ft_strchr(v.env_b, '=');
	if (v.eq_a)
		v.len_a = (int)(v.eq_a - v.env_a);
	else
		v.len_a = ft_strlen(v.env_a);
	if (v.eq_b)
		v.len_b = (int)(v.eq_b - v.env_b);
	else
		v.len_b = ft_strlen(v.env_b);
	if (v.len_a < v.len_b)
		v.cmp = ft_strncmp(v.env_a, v.env_b, v.len_a);
	else
		v.cmp = ft_strncmp(v.env_a, v.env_b, v.len_b);
	if (v.cmp == 0)
		return ((int)(v.len_a - v.len_b));
	return (v.cmp);
}

int	count_env_nodes(t_list *env)
{
	int		count;
	t_list	*node;

	count = 0;
	node = env;
	while (node)
	{
		count++;
		node = node->next;
	}
	return (count);
}

void	sort_env_array(char **env_arr, int count)
{
	int		i;
	char	*tmp;
	int		j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (env_key_cmp(&env_arr[j], &env_arr[j + 1]) > 0)
			{
				tmp = env_arr[j];
				env_arr[j] = env_arr[j + 1];
				env_arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

char	**fill_and_sort_env_array(t_list *env, int count)
{
	char	**env_arr;
	int		i;
	t_list	*node;

	node = env;
	env_arr = malloc(sizeof(char *) * count);
	if (!env_arr)
		return (NULL);
	i = 0;
	while (i < count)
	{
		env_arr[i] = node->content;
		node = node->next;
		i++;
	}
	sort_env_array(env_arr, count);
	return (env_arr);
}

void	print_env_array(char **env_arr, int count)
{
	int		i;
	char	*eq;
	int		name_len;

	i = 0;
	while (i < count)
	{
		eq = ft_strchr(env_arr[i], '=');
		if (eq)
		{
			name_len = eq - env_arr[i];
			printf("declare -x %.*s=\"%s\"\n", (int)name_len, env_arr[i], eq
				+ 1);
		}
		else
		{
			printf("declare -x %s\n", env_arr[i]);
		}
		i++;
	}
}
