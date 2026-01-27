/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 18:50:27 by njaradat          #+#    #+#             */
/*   Updated: 2025/10/02 14:55:15 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstdup(t_list *lst)
{
	t_list	*new_list;
	t_list	*new_node;
	int		*value_copy;

	new_list = NULL;
	while (lst)
	{
		value_copy = malloc(sizeof(int));
		if (!value_copy)
		{
			ft_lstclear(&new_list, free);
			return (NULL);
		}
		*value_copy = *((int *)lst->content);
		new_node = ft_lstnew(value_copy);
		if (!new_node)
		{
			free(value_copy);
			ft_lstclear(&new_list, free);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}
