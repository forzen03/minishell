/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 17:24:28 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char * get_env_value(t_list * env, char * name, int * flag)
{
	char * entry;
	while (env)
	{
		entry = env->content;
		(* flag) = 0;
		if (!ft_strncmp(entry, name, ft_strlen(name)) && entry[ft_strlen(name)] == '=')
			return (ft_strdup(entry + ft_strlen(name) + 1));
		env = env->next;
	}
	(* flag) = 1;
	return (NULL);
}

char * expand_exit_status(char * res, int * i, char * s, int * flag)
{
	char * value;
	if (s[* i] == '?')
	{
		value = ft_itoa(g_exit_status);
		if (!value)
		{
			(* flag) = 2;
			return (NULL);
		}
		(* i)++;
		res = ft_strjoin_free(res, value);
		if (!res)
		{
		 free(value);
		 (* flag) = 2;
		 return (NULL);
		}
		(* flag) = 0;
		return (res);
	}
	(* flag) = 1;
	return (res);
}

char * expand_dollar(char * res, char * s, int * i, t_list * env)
{
	char * name;
	char * value;
	int start;

	int flag;

	flag = 0;
	res = expand_exit_status(res, i, s, & flag);
	if (flag == 0)
		return (res);
	if (flag == 2)
		return (NULL);
	if (!ft_isalpha(s[* i]) && s[* i] != '_')
		return (ft_charjoin(res, '$'));
	start = * i;
	while (s[* i] && (ft_isalnum(s[* i]) || s[* i] == '_'))
		(* i)++;
	name = ft_substr(s, start, * i - start);
	if (!name)
		return (NULL);
	value = get_env_value(env, name,& flag);
	free(name);
	if (!value && flag == 1)
	{
		if (!res)
			res = ft_strdup("");
		return (res);
	}
	if (value)
		res = ft_strjoin_free(res, value);
	return (res);
}

void	expand_redirection(t_cmd * tmp, t_list * env)
{
	t_redir * r;
	char * new;
	r = tmp->redirs;
	while (r)
	{
		if (r->quote_type != 2 && r->type != TOKEN_HEREDOC)
		{
			new = expand_one_arg(r->file, env, tmp);
			if (!new)
				new = ft_strdup("");
			free(r->file);
			r->file = new;
		}
		r = r->next;
	}
}

void	rebuild_argv_loop(t_cmd * cmd, char ** new_argv, int * new_quote_types)
{
	int i;

	int j;

	j = 0;
	i = 0;
	while (cmd->argv && cmd->argv[i])
	{
		if (cmd->argv[i][0] != '\0' || cmd->quote_types[i] != 0)
		{
			new_argv[j] = cmd->argv[i];
			new_quote_types[j] = cmd->quote_types[i];
			j++;
		}
		else
			free(cmd->argv[i]);
		i++;
	}
}
