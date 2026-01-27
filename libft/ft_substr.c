/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 12:45:59 by njaradat          #+#    #+#             */
/*   Updated: 2025/08/09 18:32:22 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*p;
	size_t	i;

	if (s == NULL)
		return (NULL);
	i = 0;
	if (len == 0 || start >= ft_strlen(s))
	{
		p = (char *)malloc(sizeof(char) * 1);
		if (p == NULL)
			return (NULL);
		p[i] = '\0';
		return (p);
	}
	if (ft_strlen(s) < len + start)
		len = ft_strlen(s) - start;
	p = (char *)malloc(sizeof(char) * (len + 1));
	if (p == NULL)
		return (NULL);
	while (i < len && s[start])
		p[i++] = s[start++];
	p[i] = '\0';
	return (p);
}
