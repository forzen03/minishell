/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 12:27:35 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 16:28:26 by noorjaradat      ###   ########.fr       */
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

static int	count_env_list(t_list *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

static char	**populate_env_array(t_list *env, int count)
{
	char	**ret_env;
	int		i;

	ret_env = malloc(sizeof(char *) * (count + 1));
	if (!ret_env)
		return (NULL);
	i = 0;
	while (env && i < count)
	{
		ret_env[i] = ft_strdup((char *)env->content);
		if (!ret_env[i])
		{
			while (i > 0)
				free(ret_env[--i]);
			free(ret_env);
			return (NULL);
		}
		env = env->next;
		i++;
	}
	ret_env[count] = NULL;
	return (ret_env);
}

char	**env_list_to_array(t_list *env)
{
	int	count;

	count = count_env_list(env);
	return (populate_env_array(env, count));
}
