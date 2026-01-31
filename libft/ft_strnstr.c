/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 16:02:51 by njaradat          #+#    #+#             */
/*   Updated: 2025/08/14 13:21:35 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	little_len;
	size_t	i;
	size_t	j;
	size_t	big_len;

	if (!(*little))
		return ((char *)big);
	i = 0;
	little_len = ft_strlen(little);
	big_len = ft_strlen(big);
	while (i + little_len <= len && big[i] && i < big_len)
	{
		j = 0;
		while ((i + j < len) && (big[i + j] == little[j]) && big[i + j])
			j++;
		if (j == little_len)
			return ((char *)&big[i]);
		i++;
	}
	return (NULL);
}
