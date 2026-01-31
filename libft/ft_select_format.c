/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_select_format.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 15:40:34 by njaradat          #+#    #+#             */
/*   Updated: 2025/10/07 18:21:02 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	put_hexa(char c, va_list pa, int *counter)
{
	char	*s;

	s = NULL;
	if (c == 'x')
	{
		s = ft_xtoa(va_arg(pa, unsigned int));
		if (!s)
			return (-1);
		(*counter) += ft_strlen(s);
		ft_putstr_fd(s, 1);
	}
	else if (c == 'X')
	{
		s = ft_upper_xtoa(va_arg(pa, unsigned int));
		if (!s)
			return (-1);
		(*counter) += ft_strlen(s);
		ft_putstr_fd(s, 1);
	}
	free(s);
	return (0);
}

static void	put_str(va_list pa, int *counter)
{
	char	*s;

	s = va_arg(pa, char *);
	if (!s)
		(*counter) += write(1, "(null)", 6);
	else
	{
		(*counter) += ft_strlen(s);
		ft_putstr_fd(s, 1);
	}
}

static int	put_pointer(va_list pa, int *counter)
{
	char	*s;
	void	*p;

	p = va_arg(pa, void *);
	if (!p)
	{
		write(1, "(nil)", 5);
		(*counter) += 5;
		return (0);
	}
	s = ft_ptoa((unsigned long)p);
	if (!s)
		return (-1);
	write(1, "0x", 2);
	ft_putstr_fd(s, 1);
	(*counter) += 2 + ft_strlen(s);
	free(s);
	return (0);
}

static int	put_ui(va_list pa, int *counter, char c)
{
	char	*s;

	if (c == 'u')
		s = ft_uitoa(va_arg(pa, unsigned int));
	else
		s = ft_itoa(va_arg(pa, int));
	if (!s)
		return (-1);
	(*counter) += ft_strlen(s);
	ft_putstr_fd(s, 1);
	free(s);
	return (0);
}

int	ft_select_format(char c, va_list pa, int *counter)
{
	char	ch;
	int		protection;

	protection = 0;
	if (c == 'c')
	{
		ch = va_arg(pa, int);
		write(1, &ch, 1);
		(*counter)++;
	}
	else if (c == 's')
		put_str(pa, counter);
	else if (c == 'i' || c == 'd' || c == 'u')
		protection = put_ui(pa, counter, c);
	else if (c == '%')
	{
		write(1, "%", 1);
		(*counter)++;
	}
	else if (c == 'x' || c == 'X')
		protection = put_hexa(c, pa, counter);
	else if (c == 'p')
		protection = put_pointer(pa, counter);
	return (protection);
}
