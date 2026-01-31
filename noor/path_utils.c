/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 12:16:49 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/01/31 13:20:55 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// Get PATH value from environment
static char *get_path_from_env(t_list *env)
{
    while (env)
    {
        if (ft_strncmp((char *)env->content, "PATH=", 5) == 0)
            return ((char *)env->content + 5);
        env = env->next;
    }
    return (NULL);
}

// Free paths array from ft_split
static void free_paths_array(char **paths, int start_index)
{
    int i;
    
    i = start_index;
    while (paths[i])
    {
        free(paths[i]);
        i++;
    }
    free(paths);
}

static char *get_full_path(char **paths,char *cmd)
{
    int i;
    char *temp;
    char *full_path;

    i = 0;
    while (paths[i])
    {
        // Build full_path = dir + "/" + cmd
        temp = ft_strjoin(paths[i], "/");
        if (!temp)
            return (NULL);
        full_path = ft_strjoin(temp, cmd);
        free(temp);
        if (!full_path)
            return (NULL);
        // Check if executable
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

char *find_cmd_path(char *cmd, t_list *env)
{
    
    char **paths;
    char *path_env;
    char *full_path;

    if (!cmd || !cmd[0])
        return (NULL);
    
    // If cmd contains '/', it's absolute path
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    
    // Get PATH from environment
    path_env = get_path_from_env(env);
    if (!path_env)
        return (NULL);
    
    // Split PATH by ':'
    paths = ft_split(path_env, ':');
    if (!paths)
        return (NULL);
    full_path = get_full_path(paths,cmd);
    if (full_path)
        return (full_path);
    // Not found - or NULL - free paths array
    free_paths_array(paths, 0);
    return (NULL);
}
