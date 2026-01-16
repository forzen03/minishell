/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:43:16 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:43:16 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	join_ope(char *join, const char *s1, const char *s2)
{
	int	i;
	int	j;
	int	n;
	int	len;

	i = 0;
	j = 0;
	n = ft_strlen(s1);
	len = ft_strlen(s2);
	while (i < n)
	{
		join[i] = s1[i];
		i++;
	}
	while (j < len)
	{
		join[i] = s2[j];
		j++;
		i++;
	}
	join[i] = '\0';
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		n;
	int		len;
	char	*join;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	n = ft_strlen(s1);
	len = ft_strlen(s2);
	join = (char *)malloc(sizeof(char) * (n + len + 1));
	if (join == NULL)
		return (NULL);
	join_ope(join, s1, s2);
	return (join);
}
