/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_redirections.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 17:24:28 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_redir(t_cmd * cmd, t_token ** tokens)
{
	t_redir * node;
	node = malloc(sizeof(t_redir));
	if (!node)
		cmds_memory_allocation_failed(* tokens, cmd);
	node->type = (* tokens)->type;
	node->file = ft_strdup((* tokens)->next->value);
	if (!node->file)
		cmds_memory_allocation_failed(* tokens, cmd);
	node->quote_type = (* tokens)->next->quote_type;
	node->next = NULL;
	ft_lstadd_back_redir(& cmd->redirs, node);
	* tokens = (* tokens)->next;
}

void	ft_lstadd_back_redir(t_redir ** lst, t_redir * new)
{
	t_redir	* cur;
	if (lst == NULL || new == NULL)
		return;
	cur = * lst;
	if (cur == NULL)
	{
		* lst = new;
		return;
	}
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = new;
}

void	ft_lstclear_redirections(t_redir ** lst, void (* del)(void *))
{
	t_redir	* temp;
	t_redir	* next;
	if (lst == NULL || del == NULL)
		return;
	while (* lst != NULL)
	{
		temp = * lst;
		next = temp->next;
		del(temp->file);
		free(temp);
		* lst = next;
	}
}
