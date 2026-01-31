/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/31 12:27:35 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/01/31 12:30:04 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_env_array(char **envp)
{
    int i;
    
    if (!envp)
        return;
    i = 0;
    while (envp[i])
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}

char **env_list_to_array(t_list *env)
{
    char **ret_env;
    int count;
    int i;
    t_list *tmp;
    
    // Count environment variables
    count = 0;
    tmp = env;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    
    // Allocate array (count + 1 for NULL terminator)
    ret_env = malloc(sizeof(char *) * (count + 1));
    if (!ret_env)
        return (NULL);
    
    // Copy each string
    i = 0;
    while (env && i < count)
    {
        ret_env[i] = ft_strdup((char *)env->content);
        if (!ret_env[i])
        {
            // Free previous allocations
            while (i > 0)
                free(ret_env[--i]);
            free(ret_env);
            return (NULL);
        }
        env = env->next;
        i++;
    }
    ret_env[count] = NULL;  // NULL-terminate array
    return (ret_env);
}