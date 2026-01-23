/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:44:18 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:44:18 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int		n;
	int		last;
	int		i;

	i = 0;
	last = -1;
	if (s == NULL)
		return (NULL);
	n = (int)ft_strlen(s);
	if ((char)c == '\0')
		return ((char *)(s + n));
	while (s[i] != '\0')
	{
		if (s[i] == (char)c)
			last = i;
		i++;
	}
	if (last != -1)
		return ((char *)(s + last));
	return (NULL);
}
