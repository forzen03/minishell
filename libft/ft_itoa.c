/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:40:21 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:40:21 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long int	get_len(long int n)
{
	long int	len;

	len = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		len++;
		n = -n;
	}
	while (n > 0)
	{
		n /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int x)
{
	long int	len;
	int			cur;
	long int	n;
	char		*d;

	n = x;
	len = get_len(n);
	d = (char *)malloc((len + 1) * sizeof(char));
	if (d == NULL)
		return (NULL);
	d[len] = '\0';
	cur = 0;
	if (n < 0)
	{
		cur = 1;
		d[0] = '-';
		n = -n;
	}
	while (len > cur)
	{
		len--;
		d[len] = (n % 10) + '0';
		n /= 10;
	}
	return (d);
}

/*int main(void)
{
	int x = -123;
	printf("%s",ft_itoa(x));
	return (0);
}*/