/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:00:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 17:10:42 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_pwd_value(t_list **env, char *value)
{
	t_list	*c;

	c = *env;
	while (c)
	{
		if (ft_strcmp(c->content, value) == 0)
			return ((char *)c->content + 4);
		c = c->next;
	}
	return (NULL);
}

int	builtin_pwd(t_list **env)
{
	char	*pwd;
	char	*cwd;

	pwd = get_pwd_value(env, "PWD");
	if (pwd)
	{
		printf("%s\n", pwd);
		return (0);
	}
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("minishell: pwd");
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	return (0);
}
