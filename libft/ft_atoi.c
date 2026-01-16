/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:39:01 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:39:01 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	set(int *i, int *res, int *sign, int *count)
{
	*i = 0;
	*res = 0;
	*sign = 1;
	*count = 0;
}

int	ft_atoi(const char *s)
{
	int	i;
	int	sign;
	int	result;
	int	g;

	set(&i, &result, &sign, &g);
	while (s[i] == ' ' || (s[i] >= '\t' && s[i] <= '\r'))
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			sign *= -1;
		i++;
	}
	while (s[i] >= '0' && s[i] <= '9')
	{
		g++;
		result = result * 10 + (s[i] - '0');
		i++;
	}
	return (sign * result);
}

/*int main()
{
	int x = ft_atoi("-21547483648");
	int y = atoi("-21475483648");
	printf("%d\n",x);
	printf("%d\n",y);
}*/
