/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 16:55:00 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 16:58:38 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_numeric(char *s)
{
	int	i;

	i = 0;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')
		i++;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i] || !ft_isdigit(s[i]))
		return (0);
	while (s[i] && ft_isdigit(s[i]))
		i++;
	while (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')
		i++;
	return (s[i] == '\0');
}

static void	close_all_fds(void)
{
	int	i;

	i = 3;
	while (i < 1024)
		close(i++);
}

int	builtin_exit(char **argv, int *exit_status)
{
	ft_putstr_fd("exit\n", 1);
	if (!argv[1])
	{
		close_all_fds();
		exit(*exit_status);
	}
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	if (!is_numeric(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(argv[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		close_all_fds();
		exit(2);
	}
	close_all_fds();
	exit(ft_atoi(argv[1]) % 256);
}
