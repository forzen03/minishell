/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_delete_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 00:00:00 by njaradat          #+#    #+#             */
/*   Updated: 2026/01/27 17:37:14 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_delete_node(t_list **lst, void *content, void (*del)(void*))
{
	t_list	*current;
	t_list	*prev;

	if (!lst || !*lst)
		return (1);
	current = *lst;
	prev = NULL;
	while (current)
	{
		if (current->content == content)
		{
			if (prev == NULL)
				*lst = current->next;
			else
				prev->next = current->next;
			if (del)
				del(current->content);
			free(current);
			return (0);
		}
		prev = current;
		current = current->next;
	}
	return (1);
}
