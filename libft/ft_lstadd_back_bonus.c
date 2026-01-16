/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 15:40:27 by mqadah            #+#    #+#             */
/*   Updated: 2025/08/09 15:40:27 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*cur;

	if (lst == NULL || new == NULL)
		return ;
	cur = *lst;
	if (cur == NULL)
	{
		*lst = new;
		return ;
	}
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = new;
}
