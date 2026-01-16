/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:42:02 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:42:02 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*dst;
	const unsigned char	*sr;

	dst = (unsigned char *)dest;
	sr = (const unsigned char *)src;
	if (dst > sr)
	{
		while (n > 0)
		{
			n--;
			dst[n] = sr[n];
		}
	}
	else
		ft_memcpy(dest, src, n);
	return (dest);
}

/*int main()
{
	char t[6] = "12345";
	char r[6] = "12345";
	memmove(t+2,t,3);
	ft_memmove(r+2,r,3);
	printf("ori: %s\n",t);
	printf("mine:%s\n",r);
}*/