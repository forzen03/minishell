/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 16:17:06 by njaradat          #+#    #+#             */
/*   Updated: 2025/10/01 23:23:17 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*cur;
	t_list	*next;

	if (!lst)
		return ;
	while (*lst != NULL)
	{
		cur = *lst;
		next = cur->next;
		if (del && cur->content)
			del(cur->content);
		free(cur);
		*lst = next;
	}
}
