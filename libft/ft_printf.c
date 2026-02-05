/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 14:48:15 by njaradat          #+#    #+#             */
/*   Updated: 2025/10/07 18:19:51 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_string(const char *s, va_list ap, int *counter)
{
	int	i;
	int	len;
	int	protection;

	protection = 0;
	len = (int)ft_strlen(s);
	i = 0;
	while (i < len)
	{
		if (s[i] == '%')
		{
			i++;
			protection = ft_select_format(s[i], ap, counter);
			if (protection)
				return (-1);
		}
		else
		{
			write(1, &s[i], 1);
			(*counter)++;
		}
		i++;
	}
	return (protection);
}

int	ft_printf(const char *s, ...)
{
	va_list	ap;
	int		counter;
	int		protection;

	if (!s)
		return (-1);
	protection = 0;
	counter = 0;
	va_start(ap, s);
	protection = print_string(s, ap, &counter);
	if (protection)
		return (-1);
	va_end(ap);
	return (counter);
}
