/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:44:26 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:44:26 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	get_size_front(const char *s1, const char *set)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (set[j] != '\0')
	{
		if (s1[i] == set[j] && s1[i])
		{
			i++;
			j = -1;
		}
		j++;
	}
	return (i);
}

static int	get_size_last(const char *s1, const char *set)
{
	int	i;
	int	j;

	j = 0;
	i = (int)ft_strlen(s1) - 1;
	while (set[j] != '\0')
	{
		if (s1[i] == set[j] && s1[i])
		{
			i--;
			j = -1;
		}
		j++;
	}
	return (i);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*dst;
	int		tot;
	int		start;
	int		end;

	if (s1 == NULL || set == NULL)
		return (NULL);
	start = get_size_front(s1, set);
	end = get_size_last(s1, set);
	if (start > end)
	{
		dst = ft_strdup("");
		return (dst);
	}
	tot = end - start + 1;
	dst = ft_substr(s1, start, tot);
	return (dst);
}
/*


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libft.h"

int main(void)
{
    char *res;

    // 1 - No trim chars in string
    res = ft_strtrim("abcdef", "xyz");
    printf("1: '%s'\n", res);
    free(res);

    // 2 - Trim set is empty (should return the string unchanged)
    res = ft_strtrim("abcdef", "");
    printf("2: '%s'\n", res);
    free(res);

    // 3 - String is empty, set not empty (should return "")
    res = ft_strtrim("", "abc");
    printf("3: '%s'\n", res);
    free(res);

    // 4 - String and set both empty (should return "")
    res = ft_strtrim("", "");
    printf("4: '%s'\n", res);
    free(res);

    // 5 - Entire string is set chars (should return "")
    res = ft_strtrim("aaaaaa", "a");
    printf("5: '%s'\n", res);
    free(res);

    // 6 - Trim only front
    res = ft_strtrim("aaabc", "a");
    printf("6: '%s'\n", res);
    free(res);

    // 7 - Trim only back
    res = ft_strtrim("bcaaa", "a");
    printf("7: '%s'\n", res);
    free(res);

    // 8 - Set contains multiple chars
    res = ft_strtrim("xyxyHello Worldyx", "xy");
    printf("8: '%s'\n", res);
    free(res);

    // 9 - Set contains spaces
    res = ft_strtrim("   hello world",NULL);
    printf("9: '%s'\n", res);
    free(res);

    // 10 - String contains only set chars in middle but not trim area
    res = ft_strtrim("aaXaa", "a");
    printf("10: '%s'\n", res);
    free(res);

    // 11 - Weird symbols
    res = ft_strtrim("$$$Hello$$$", "$");
    printf("11: '%s'\n", res);
    free(res);

    // 12 - One char string, char in set
    res = ft_strtrim("a", "a");
    printf("12: '%s'\n", res);
    free(res);

    // 13 - One char string, char not in set
    res = ft_strtrim("b", "a");
    printf("13: '%s'\n", res);
    free(res);

    return 0;
}*/
