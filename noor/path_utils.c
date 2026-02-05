/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 12:16:49 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 16:38:51 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*get_path_from_env(t_list *env)
{
	while (env)
	{
		if (ft_strncmp((char *)env->content, "PATH=", 5) == 0)
			return ((char *)env->content + 5);
		env = env->next;
	}
	return (NULL);
}

static void	free_paths_array(char **paths, int start_index)
{
	int	i;

	i = start_index;
	while (paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

static char	*get_full_path(char **paths, char *cmd)
{
	int		i;
	char	*temp;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (NULL);
		if (access(full_path, X_OK) == 0)
		{
			free_paths_array(paths, 0);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*find_cmd_path(char *cmd, t_list *env)
{
	char	**paths;
	char	*path_env;
	char	*full_path;

	if (!cmd || !cmd[0])
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_path_from_env(env);
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full_path = get_full_path(paths, cmd);
	if (full_path)
		return (full_path);
	free_paths_array(paths, 0);
	return (NULL);
}
