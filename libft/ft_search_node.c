/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_search_node.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaradat <njaradat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/27 00:00:00 by njaradat          #+#    #+#             */
/*   Updated: 2026/01/27 17:27:22 by njaradat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_search_node(t_list *lst, void *content)
{
	while (lst)
	{
		if (lst->content == content)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}
