/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 14:11:46 by njaradat          #+#    #+#             */
/*   Updated: 2025/08/12 11:33:22 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_in_set(char c, char const *set)
{
	size_t	i;

	i = 0;
	while (set[i])
	{
		if ((char)c == set[i])
			return (1);
		i++;
	}
	return (0);
}

static size_t	get_start_index(char const *s1, char const *set, size_t *len)
{
	size_t	i;

	i = 0;
	while (is_in_set(s1[i], set))
	{
		i++;
		(*len)--;
	}
	return (i);
}

static void	get_end_index(char const *s1, char const *set, size_t *len)
{
	size_t	end;

	end = ft_strlen(s1) - 1;
	while (is_in_set(s1[end], set))
	{
		end--;
		(*len)--;
	}
}

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len;
	size_t	start;
	char	*p;

	if (s1 == NULL || set == NULL)
		return (NULL);
	if (!(*set))
	{
		p = ft_strdup(s1);
		if (p == NULL)
			return (NULL);
		return (p);
	}
	len = ft_strlen(s1);
	start = get_start_index(s1, set, &len);
	get_end_index(s1, set, &len);
	p = ft_substr(s1, start, len);
	return (p);
}
