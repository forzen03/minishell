/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:39:19 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:39:19 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t			tot;
	void			*p;

	if (nmemb == 0 || size == 0)
	{
		p = malloc(sizeof(char) * 1);
		if (p == NULL)
			return (NULL);
		return (p);
	}
	tot = nmemb * size;
	if (tot / size != nmemb || tot / nmemb != size)
		return (NULL);
	p = malloc(sizeof(char) * tot);
	if (p == NULL)
		return (NULL);
	ft_bzero(p, tot);
	return (p);
}

/*int main(void)
{
    void *ptr;

    // ... other test cases ...

    // Overflow test case - extremely large allocation
    ptr = ft_calloc(13123131313UL, 3131231313132131313UL);
    printf("Overflow test: %p\n", ptr);
    // Expected output: (nil) or 0x0 (meaning NULL)

    if (ptr)
        free(ptr);

    return 0;
}*/
