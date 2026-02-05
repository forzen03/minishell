/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer5.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 17:18:48 by noorjaradat       #+#    #+#             */
/*   Updated: 2026/02/05 17:19:29 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	return_value_of_quotes(int has_double, int has_single, int has_unquoted)
{
	if (has_unquoted || (has_double && has_single))
		return (0);
	if (has_double)
		return (1);
	if (has_single)
		return (2);
	return (0);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	int		n;
	int		len;
	char	*join;

	if (s1 == NULL && s2 == NULL)
		return (NULL);
	if (s1 == NULL)
		return (s2);
	if (s2 == NULL)
		return (s1);
	n = ft_strlen(s1);
	len = ft_strlen(s2);
	join = (char *)malloc(sizeof(char) * (n + len + 1));
	if (join == NULL)
		return (NULL);
	join_ope(join, s1, s2);
	free(s1);
	free(s2);
	return (join);
}
