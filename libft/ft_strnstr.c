/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:44:10 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:44:10 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	find_str(const char *s1, const char *s2, size_t len)
{
	size_t	i;
	int		j;
	size_t	n;
	size_t	t;

	i = 0;
	while (i < len && s1[i] != '\0')
	{
		t = i;
		j = 0;
		n = ft_strlen(s2);
		while (s1[t] == s2[j] && t < len && s1[t])
		{
			t++;
			j++;
			n--;
			if (n == 0)
				return ((int)i);
		}
		i++;
	}
	return (-1);
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t	n;
	int		i;

	n = ft_strlen(needle);
	if (n == 0)
		return ((char *)haystack);
	i = find_str(haystack, needle, len);
	if (i != -1)
		return ((char *)(haystack + i));
	return (NULL);
}

/*int main(void)
{
    const char *haystack = "Hello, 42 School!";
    const char *needle = "";

    char *result = ft_strnstr(haystack, needle, 10);

    if (result == haystack)
        printf("Test passed: returned haystack when needle is empty.\n");
    else
        printf("Test failed: did not return haystack.\n");

    printf("Result points to: \"%s\"\n", result);

    return 0;
}*/