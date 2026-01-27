/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 16:21:51 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/01/27 18:44:24 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void update_env(t_list **env, char *key, char *value)
{
    t_list *cur;
    t_list *new;
    cur = *env;
    while (cur)
    {
        if (ft_strncmp(cur->content, key, ft_strlen(key)) == 0)
        {
            free(cur->content);
            cur->content = ft_strjoin(key, value); // key must include '='
            return;
        }
        cur = cur->next;
    }
    new = malloc(sizeof(t_list));
    new->content = ft_strjoin(key, value); // key must include '='
    ft_lstadd_back(env,new);
}


int is_variable(char *s)
{
    int i;

    i = 0;
    if (s[i] == '=')
        return 0;
    if (!ft_isalpha(s[i]))
        return 0;
    while (s[i])
    {
        if (!ft_isalnum(s[i]) && s[i] != '_')
            return 0;
        i++;
    }
    return 1;
}

// 1. cd builtin

void set_env(t_list *env,const char *key, const char *value)
{
    t_list *cur;

    cur = env;
    while (cur)
    {
        if (ft_strncmp((char *)cur->content,key,ft_strlen(key)) == 0)
        {
            free(cur->content);
            cur->content = ft_strjoin(key,value);
            break;
        }
        cur = cur->next;
    }
}

int builtin_cd(char **argv, t_list *env)
{
    // Change directory to argv[1]
    // Update PWD in environment
    // Return: 0 on success, 1 on error
    char *oldpwd;
    char *newpwd;
    char *path;
    t_list *cur;

    oldpwd = getcwd(NULL,0);
    if (!oldpwd)
        return 1;
    path = argv[1];
    if (!path)
    {
        cur = env;
        while (cur)
        {
            if (ft_strncmp((char *)cur->content,"HOME=",5) == 0)
            {
                path = (char *)cur->content + 5;
                break;
            }
            cur = cur->next;
        }
    }
    if (!path || chdir(path) == -1)//change directory
    {
        perror("cd");
        free(oldpwd);
        return 1;
    }
    newpwd = getcwd(NULL,0);
    if (!newpwd)
    {
        free(oldpwd);
        return 1;
    }
    //set env
    set_env(env,"PWD=",newpwd);
    set_env(env,"OLDPWD=",oldpwd);
    free(oldpwd);
    free(newpwd);
    return 0;
}

// 2. export builtin

int builtin_export(char **argv, t_list **env)
{
    int i;
    char *key;
    char *value;
    char *equal;

    i = 1;
    while (argv[i])
    {
        equal = ft_strchr(argv[i],'=');
        if (!equal)
            break;
        if (equal + 1)
        {
            key = ft_substr(argv[i],0,equal - argv[i]);
            value = ft_strdup(equal + 1);
        }
        else if (!(equal + 1))
        {
            key = ft_strdup(argv[i]);
            value = "";
        }
        if (!is_variable(key))
        {
            perror("export: not a valid identifier\n");
            free(key);
            free(value);
            return 1;
        }
        key = ft_strjoin(key,"=");
        update_env(env,key,value);
        free(key);
        free(value);
        i++;
    }
    return 0;
    // Add/modify environment variable
    // Format: export VAR=value
    // Return: 0 on success, 1 on error
}

// 3. unset builtin


int	delete_node_env(t_list **lst, void *content, void (*del)(void*))
{
	t_list	*current;
	t_list	*prev;

	if (!lst || !*lst)
		return (1);
	current = *lst;
	prev = NULL;
	while (current)
	{
		if (current->content == content)
		{
			if (prev == NULL)
				*lst = current->next;
			else
				prev->next = current->next;
			if (del)
				del(current->content);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}


int builtin_unset(char **argv, t_list **env)
{
    // unset var1 var2 var3
    // Remove environment variable
    // Format: unset VAR
    // Return: 0 on success, 1 on error
    char *key;
    int i;
    t_list *cur;

    i = 1;
    if (!argv[i])//unset alone with no argv
        return (1);
    key = argv[i];
    while (key)
    {
        cur = *env;
        while (cur)
        {
            if (ft_strncmp((char *)cur->content, key, ft_strlen(key)) == 0)// PATH=user/bin   key = PATH
                ft_delete_node(env, cur->content, free);
            cur = cur->next;
        }
        i++;
        key = argv[i];
    }
    return (0);
}

// 4. exit builtin


int is_numeric(char *s)
{
    int i = 0;

    if (s[i] == '+' || s[i] == '-')
        i++;
    if (!s[i])
        return (0);
    while (s[i])
    {
        if (!ft_isdigit(s[i]))
            return (0);
        i++;
    }
    return (1);
}

int builtin_exit(char **argv)
{
    int i = 0;

    ft_putstr_fd("exit\n", 1);

    // exit
    if (!argv[1])
        exit(g_exit_status);

    // too many arguments
    if (argv[2])
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        g_exit_status = 1;
        return (1);
    }

    // numeric validation
    if (!is_numeric(argv[1]) || ft_atoi(argv[1]) < 0)
    {
        ft_putstr_fd("minishell: exit: ", 2);
        ft_putstr_fd(argv[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        exit(255);
    }

    // valid numeric
    exit(ft_atoi(argv[1]) % 256);
}

// 5. pwd builtin
int builtin_pwd(void)
{
    return (1);
    // Print current working directory
    // Return: 0 on success, 1 on error
}

// 6. echo builtin
int builtin_echo(char **argv)
{

    (void)argv;
    return (1);
    // Print arguments (handle -n flag)
    // Format: echo [-n] args...
    // Return: 0 always
}

// 7. env builtin
int builtin_env(t_list *env)
{

    (void)env;
    return (1);
    // Print all environment variables
    // Return: 0 on success, 1 on error
}
