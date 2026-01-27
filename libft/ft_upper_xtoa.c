/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_upper_xtoa.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 17:02:08 by njaradat          #+#    #+#             */
/*   Updated: 2025/10/07 18:19:25 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_hex_len(unsigned int n)
{
	int	count;

	count = 0;
	while (n > 0)
	{
		n /= 16;
		count++;
	}
	return (count);
}

char	*ft_upper_xtoa(unsigned int n)
{
	char			*s;
	size_t			i;
	unsigned int	temp;

	if (n == 0)
		return (ft_strdup("0"));
	i = ft_hex_len(n);
	s = (char *)malloc(sizeof(char) * (i + 1));
	if (!s)
		return (NULL);
	s[i--] = '\0';
	while (n != 0)
	{
		temp = n % 16;
		if (temp < 10)
			s[i--] = temp + '0';
		else
			s[i--] = temp + 55;
		n /= 16;
	}
	return (s);
}
