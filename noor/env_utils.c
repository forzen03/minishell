/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 12:27:35 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 13:51:45 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env_array(char **envp)
{
	int	i;

	if (!envp)
		return ;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static int	fill_env_array(char **ret_env, t_list *env, int count)
{
	int	i;

	i = 0;
	while (env && i < count)
	{
		ret_env[i] = ft_strdup((char *)env->content);
		if (!ret_env[i])
		{
			while (i > 0)
				free(ret_env[--i]);
			return (0);
		}
		env = env->next;
		i++;
	}
	ret_env[count] = NULL;
	return (1);
}

char	**env_list_to_array(t_list *env)
{
	char	**ret_env;
	int		count;
	t_list	*tmp;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	ret_env = malloc(sizeof(char *) * (count + 1));
	if (!ret_env)
		return (NULL);
	if (!fill_env_array(ret_env, env, count))
	{
		free(ret_env);
		return (NULL);
	}
	return (ret_env);
}
