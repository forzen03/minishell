/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 14:31:16 by njaradat          #+#    #+#             */
/*   Updated: 2025/08/14 12:55:50 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	int	slen;

	slen = (int)ft_strlen(s);
	if ('\0' == (char)c)
		return ((char *)s + slen);
	slen--;
	while (slen >= 0)
	{
		if (s[slen] == (char)c)
			return ((char *)s + slen);
		slen--;
	}
	return (NULL);
}
