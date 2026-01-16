/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstsize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:41:33 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:41:33 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	int		i;
	t_list	*cur;

	cur = lst;
	i = 0;
	if (cur == NULL)
		return (0);
	while (cur->next != NULL)
	{
		cur = cur->next;
		i++;
	}
	cur = NULL;
	i++;
	return (i);
}
