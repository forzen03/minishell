/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/23 16:21:51 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/03 14:14:29 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void update_env(t_list **env, char *key, char *value)
{
    t_list *cur;
    t_list *new;
    char *full_value;
    
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
    // Add new variable to list
    full_value = ft_strjoin(key, value);
    new = ft_lstnew(full_value); // Properly initialize node
    if (new)
        ft_lstadd_back(env, new);
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

void set_env(t_list **env,const char *key, const char *value)
{
    t_list *cur;

    cur = *env;
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

int builtin_cd(char **argv, t_list **env)
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
        cur = *env;
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
    update_env(env,"PWD=",newpwd);
    update_env(env,"OLDPWD=",oldpwd);
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
    if (!argv[1])  // export with no args: print all env vars
        return (0);
    while (argv[i])
    {
        equal = ft_strchr(argv[i],'=');
        if (!equal)
        {
            i++;
            continue;  // Skip variables without '='
        }
        if (*(equal + 1))
        {
            key = ft_substr(argv[i],0,equal - argv[i]);
            value = ft_strdup(equal + 1);
        }
        else
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
            if (ft_strncmp((char *)cur->content, key, ft_strlen(key)) == 0 
                && ((char *)cur->content)[ft_strlen(key)] == '=')// PATH=user/bin   key = PATH
            {
                ft_delete_node(env, cur->content, free);
                break;  // Exit inner loop after deleting to avoid using freed memory
            }
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
        return (0);//+ exit status (g_exit_status) (example :if malloc fail ->return exit status + show error )
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
    if (!is_numeric(argv[1]))
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

static char *get_pwd_value(t_list **env,char *value)
{
    t_list *c;

    c = *env;
    while (c)
    {
        if (ft_strcmp(c->content ,value) == 0)//PWD=/home/gkjdagj
            return ((char *)c->content + 4);// /home/gkjahdgjk
        c = c->next;
    }
    return (NULL);
}

int builtin_pwd(t_list **env)
{
    // Print current working directory
    // Return: 0 on success, 1 on error
    char *pwd;
    char *cwd;

    pwd = get_pwd_value(env, "PWD");// /home/gkjahdgjk
    if (pwd)
    {
        int i = -1;
        while (env[++i])
            printf("aa: %s",(char *)env[i]->content);
        printf("%s\n", pwd);
        return (0);
    }
    cwd = getcwd(NULL, 0);// getcwd malloced
    if (!cwd)
    {
        perror("minishell: pwd");
        return (1);
    }
    printf("%s\n", cwd);
    free(cwd);
    return (0);
}

// 6. echo builtin
int builtin_echo(char **argv)
{
    // Print arguments (handle -n flag)
    // Format: echo [-n] args...
    // Return: 0 always
    int new_line;
    int i;

    if (!argv[1])
    {
        printf("\n");
        return (0);
    }
    new_line = 1;
    i = 1;
    if (argv[1] && ft_strcmp(argv[1],"-n") == 0)
    {
        new_line = 0;
        i = 2;
    }
    while (argv[i])
    {
        printf("%s",argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    if (new_line)
        printf("\n");
    return (0);
}

// 7. env builtin
int builtin_env(t_list *env)
{
    // Print all environment variables
    // Return: 0 on success, 1 on error
    while (env)
    {
        printf("%s\n",(char *)env->content);
        env = env->next;
    }
    return (0);
}
