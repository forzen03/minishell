/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 16:55:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 16:58:38 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_cd_path(char **argv, t_list **env)
{
	char	*path;
	t_list	*cur;

	path = argv[1];
	if (!path)
	{
		cur = *env;
		while (cur)
		{
			if (ft_strncmp((char *)cur->content, "HOME=", 5) == 0)
			{
				path = (char *)cur->content + 5;
				break ;
			}
			cur = cur->next;
		}
	}
	return (path);
}

static int	change_directory_and_update(char *path, t_list **env, char *oldpwd)
{
	char	*newpwd;

	if (!path || chdir(path) == -1)
	{
		perror("cd");
		free(oldpwd);
		return (1);
	}
	newpwd = getcwd(NULL, 0);
	if (!newpwd)
	{
		free(oldpwd);
		return (1);
	}
	update_env(env, "PWD=", newpwd);
	update_env(env, "OLDPWD=", oldpwd);
	free(oldpwd);
	free(newpwd);
	return (0);
}

int	builtin_cd(char **argv, t_list **env)
{
	char	*oldpwd;
	char	*path;

	oldpwd = getcwd(NULL, 0);
	if (!oldpwd)
		return (1);
	path = get_cd_path(argv, env);
	return (change_directory_and_update(path, env, oldpwd));
}
