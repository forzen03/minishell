/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/02/03 18:17:24 by marvin           ###   ########.fr       */
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

char	*expand_exit_status(char *res, int *i, char *s, t_cmd *cmds)
{
	char	*value;

	if (s[*i + 1] == '?')
	{
		value = ft_itoa(cmds->exit_status);
		if (!value)
			return (NULL);
		(*i) += 2;
		res = ft_strjoin_free(res, value);
		if (!res)
			return (NULL);
		return (res);
	}
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
	(*i)++;
	if (!ft_isalpha(s[*i]) && s[*i] != '_')
	{
		res = ft_charjoin(res, '$');
		return (res);
	}
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

void	expand_redirection(t_cmd *tmp, t_list *env, t_cmd *cmds)
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
			if (!new)
				memory_allocation_failed_expand(cmds, env);
			free(r->file);
			r->file = new;
		}
		r = r->next;
	}
}
