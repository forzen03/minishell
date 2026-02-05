/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: noorjaradat <noorjaradat@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:40:41 by njaradat          #+#    #+#             */
/*   Updated: 2025/10/01 23:10:11 by noorjaradat      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	void	*temp_content;
	t_list	*new_node;

	new_list = NULL;
	if (!lst || !f || !del)
		return (NULL);
	while (lst)
	{
		temp_content = f(lst->content);
		if (!temp_content)
		{
			ft_lstclear(&new_list, free);
			return (NULL);
		}
		new_node = ft_lstnew(temp_content);
		if (!new_node)
		{
			ft_lstclear(&new_list, free);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}
