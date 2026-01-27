/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ptoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 18:11:52 by njaradat          #+#    #+#             */
/*   Updated: 2025/10/07 18:19:54 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static unsigned long	len(unsigned long n)
{
	unsigned long	count;

	count = 0;
	while (n > 0)
	{
		n /= 16;
		count++;
	}
	return (count);
}

char	*ft_ptoa(unsigned long n)
{
	char			*s;
	unsigned long	i;
	unsigned long	temp;

	i = len(n);
	s = (char *)malloc(sizeof(char) * (i + 1));
	if (!s)
		return (NULL);
	s[i--] = '\0';
	while (n != 0)
	{
		temp = n % 16;
		if (temp < 10)
			temp = temp + '0';
		else
			temp = temp + 87;
		s[i--] = temp;
		n = n / 16;
	}
	return (s);
}
