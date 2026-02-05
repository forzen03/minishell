/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 16:55:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 16:58:38 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	parse_export_argument(char *arg, char **key, char **value)
{
	char	*equal;

	equal = ft_strchr(arg, '=');
	if (!equal)
		return (0);
	if (*(equal + 1))
	{
		*key = ft_substr(arg, 0, equal - arg);
		*value = ft_strdup(equal + 1);
	}
	else
	{
		*key = ft_substr(arg, 0, equal - arg);
		*value = ft_strdup("");
	}
	return (1);
}

static int	validate_and_export(char *key, char *value, t_list **env)
{
	char	*temp;

	if (!is_variable(key))
	{
		perror("export: not a valid identifier\n");
		free(key);
		free(value);
		return (1);
	}
	temp = key;
	key = ft_strjoin(key, "=");
	free(temp);
	update_env(env, key, value);
	free(key);
	free(value);
	return (0);
}

static int	process_export_arg(char *arg, t_list **env)
{
	char	*key;
	char	*value;

	if (!parse_export_argument(arg, &key, &value))
		return (0);
	return (validate_and_export(key, value, env));
}

int	builtin_export(char **argv, t_list **env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!argv[1])
		return (0);
	while (argv[i])
	{
		if (process_export_arg(argv[i], env) != 0)
			status = 1;
		i++;
	}
	return (status);
}
