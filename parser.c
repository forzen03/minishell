#include "minishell.h"

t_cmd *parser(t_token *tokens)
{
	t_cmd *node;
	t_cmd *cmds;
	t_cmd *prev;

	if (check_syntax_error(&tokens))
		return (NULL);
	cmds = NULL;
	prev = NULL;

	while (tokens)
	{
		node = create_parser_node(tokens, cmds);

		if (prev)
			node->pipe_in = 1;
		filling_parser_nodes(&tokens, &cmds, node);

		if (tokens && tokens->type == TOKEN_PIPE)
		{
			node->pipe_out = 1;
			tokens = tokens->next;
		}
		ft_lstadd_back_cmds(&cmds, node);
		prev = node;
	}
	return (cmds);
}


void filling_parser_nodes(t_token **tokens, t_cmd **cmds, t_cmd *node)
{
	int i;

	i = 0;
	while (*tokens && (*tokens)->type != TOKEN_PIPE)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			node->argv[i] = ft_strdup((*tokens)->value);

			if (!node->argv[i])
				cmds_memory_allocation_failed(*tokens, *cmds);

			node->quote_types[i] = (*tokens)->quote_type;
			(i)++;
			*tokens = (*tokens)->next;
		}
		else if (is_redirection((*tokens)->type))
		{
			add_redir(node, tokens);
			*tokens = (*tokens)->next;
		}
		else
			*tokens = (*tokens)->next;
	}
	node->argv[i] = NULL;
}

t_cmd *create_parser_node(t_token *tokens, t_cmd *cmds)
{

	t_cmd *node;
	int len;

	len = 0;
	node = malloc(sizeof(t_cmd));

	if (!node)
		cmds_memory_allocation_failed(tokens, cmds);

	intit_parser_node(&node);

	len = argv_len(tokens);

	node->argv = malloc(sizeof(char *) * (len + 1));

	if (!node->argv)
			cmds_memory_allocation_failed(tokens, node);

	node->quote_types = malloc(sizeof(int) * len);

	if (!node->quote_types)
		cmds_memory_allocation_failed(tokens, cmds);
	return (node);
}