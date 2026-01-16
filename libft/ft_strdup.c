/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:43:04 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:43:04 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	size_t	i;
	size_t	tot;
	char	*p;

	p = NULL;
	i = 0;
	tot = ft_strlen(s);
	p = (char *)malloc(sizeof(char) * (tot + 1));
	if (p == NULL)
		return (NULL);
	while (i < tot)
	{
		p[i] = s[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}
