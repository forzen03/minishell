/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 22:08:29 by njaradat          #+#    #+#             */
/*   Updated: 2025/08/12 11:37:47 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	number_of_digit(long res)
{
	int	i;

	i = 0;
	if (res == 0)
		return (1);
	if (res < 0)
		i++;
	while (res != 0)
	{
		i++;
		res /= 10;
	}
	return (i);
}

static void	is_negative(long *res, int *flag)
{
	if (*res < 0)
	{
		*res = *res * -1;
		*flag = 1;
	}
}

char	*ft_itoa(int n)
{
	char	*p;
	int		digits;
	long	res;
	int		flag;

	flag = 0;
	res = n;
	digits = number_of_digit(res);
	is_negative(&res, &flag);
	p = (char *)malloc(sizeof(char) * (digits + 1));
	if (p == NULL)
		return (NULL);
	if (flag == 1)
		p[0] = '-';
	p[digits--] = '\0';
	while (digits >= flag)
	{
		p[digits--] = (res % 10) + '0';
		res /= 10;
	}
	return (p);
}
