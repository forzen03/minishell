/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 16:21:51 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 17:04:28 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	builtin_unset(char **argv, t_list **env)
{
	char	*key;
	int		i;
	t_list	*cur;

	i = 1;
	if (!argv[i])
		return (1);
	key = argv[i];
	while (key)
	{
		cur = *env;
		while (cur)
		{
			if (ft_strncmp((char *)cur->content, key, ft_strlen(key)) == 0
				&& ((char *)cur->content)[ft_strlen(key)] == '=')
			{
				ft_delete_node(env, cur->content, free);
				break ;
			}
			cur = cur->next;
		}
		i++;
		key = argv[i];
	}
	return (0);
}

int	builtin_echo(char **argv)
{
	int	new_line;
	int	i;

	if (!argv[1])
	{
		printf("\n");
		return (0);
	}
	new_line = 1;
	i = 1;
	if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
	{
		new_line = 0;
		i = 2;
	}
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (new_line)
		printf("\n");
	return (0);
}

int	builtin_env(t_list *env)
{
	while (env)
	{
		printf("%s\n", (char *)env->content);
		env = env->next;
	}
	return (0);
}
