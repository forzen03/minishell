/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utliltes.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 17:24:28 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_lstlast_tokens(t_token *lst)
{
	t_token	*cur;
	cur = lst;
	if (cur == NULL)
		return (NULL);
	while (cur->next != NULL)
		cur = cur->next;
	return (cur);
}

void cmds_memory_allocation_failed(t_token *tokens, t_cmd *cmds)
{
	int i;
	t_cmd *tmp;
	while (cmds)
	{
		i = 0;
		if (cmds->quote_types)
			free(cmds->quote_types);
		if (cmds->redirs)
			ft_lstclear_redirections(&cmds->redirs, free);
		if (cmds->argv)
		{
			while (cmds->argv[i])
			{
				free(cmds->argv[i]);
				i++;
			}
			free(cmds->argv);
		}
		tmp = cmds->next;
		free(cmds);
		cmds = tmp;
	}
	tokens_memory_allocation_failed(&tokens);
}

void	ft_lstadd_back_cmds(t_cmd **lst, t_cmd *new)
{
	t_cmd	*cur;
	if (lst == NULL || new == NULL)
		return;
	cur = *lst;
	if (cur == NULL)
	{
		*lst = new;
		return;
	}
	while (cur->next != NULL)
		cur = cur->next;
	cur->next = new;
}

void intit_parser_node(t_cmd **node)
{
	t_cmd *tmp;
	tmp = *node;
	tmp->redirs = NULL;
	tmp->pipe_in = 0;
	tmp->pipe_out = 0;
	tmp->next = NULL;
}

int argv_len(t_token *tokens)
{
	t_token * tmp;
	int len;
	len = 0;
	tmp = tokens;
	while (tmp && tmp->type != TOKEN_PIPE)//agvs len
	{
		if (tmp->type == TOKEN_WORD)
			len++;
		tmp = tmp->next;
	}
	return (len);
}
