/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:58:44 by njaradat          #+#    #+#             */
/*   Updated: 2025/10/07 18:19:30 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	number_of_digit(unsigned long res)
{
	int	i;

	i = 0;
	if (res == 0)
		return (1);
	while (res != 0)
	{
		i++;
		res /= 10;
	}
	return (i);
}

char	*ft_uitoa(unsigned int n)
{
	char			*p;
	int				digits;
	unsigned long	res;

	res = n;
	digits = number_of_digit(res);
	p = (char *)malloc(sizeof(char) * (digits + 1));
	if (p == NULL)
		return (NULL);
	p[digits--] = '\0';
	while (digits >= 0)
	{
		p[digits--] = (res % 10) + '0';
		res /= 10;
	}
	return (p);
}
