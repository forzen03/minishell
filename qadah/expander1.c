/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 18:14:21 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_list *env, char *name, int *flag)
{
	char	*entry;
	size_t	len;

	len = ft_strlen(name);
	while (env)
	{
		entry = (char *)env->content;
		if (entry && !ft_strncmp(entry, name, len) && entry[len] == '=')
		{
			*flag = 0;
			return (ft_strdup(entry + len + 1));
		}
		env = env->next;
	}
	*flag = 1;
	return (NULL);
}

char	*expand_exit_status(char *res, int *i, char *s, int *flag)
{
	char	*value;
	char	*old;

	if (s[*i] == '?')
	{
		value = ft_itoa(g_exit_status);
		if (!value)
		{
			*flag = 2;
			return (NULL);
		}
		(*i)++;
		old = res;
		res = ft_strjoin_free(res, value);
		if (!res)
		{
			free_two_strings(old, value);
			*flag = 2;
			return (NULL);
		}
		*flag = 0;
		return (res);
	}
	*flag = 1;
	return (res);
}

char	*expand_dollar_con(int *i, char *s, t_list *env, int *flag)
{
	int		start;
	char	*name;
	char	*value;

	start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	name = ft_substr(s, start, *i - start);
	if (!name)
	{
		*flag = 2;
		return (NULL);
	}
	value = get_env_value(env, name, flag);
	free(name);
	return (value);
}

char	*expand_dollar(char *res, char *s, int *i, t_list *env)
{
	char	*value;
	int		flag;

	flag = 0;
	res = expand_exit_status(res, i, s, &flag);
	if (flag == 2)
		return (NULL);
	if (flag == 0)
		return (res);
	if (!ft_isalpha(s[*i]) && s[*i] != '_')
		return (ft_charjoin(res, '$'));
	value = expand_dollar_con(i, s, env, &flag);
	if (flag == 2)
		return (NULL);
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

void	expand_redirection(t_cmd *tmp, t_list *env)
{
	t_redir	*r;
	char	*new;

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
