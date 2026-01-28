/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mqadah <mqadah@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/28 17:24:28 by mqadah            #+#    #+#             */
/*   Updated: 2026/01/28 17:24:28 by mqadah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_exit_status = 0;

// typedef struct s_redir
// {
//     t_token_type    type;
//     char            *file;
//     int         quote_type;
//     struct s_redir  *next;
// }   t_redir;

// typedef struct s_cmd
// {
//     char            **argv;         // NULL-terminated array
//     int             *quote_types;   // Parallel array (same size)//for expander
//     t_redir         *redirs;
//     int             pipe_in;
//     int             pipe_out;
//     struct s_cmd    *next;
// }   t_cmd;

// void print_redirs(t_redir *redir)
// {
//     while (redir)
//     {
//         char *type_str;
//         char *type_name;
//         if (redir->type == TOKEN_REDIR_IN)
//         {
//             type_str = "<";
//             type_name = "REDIR_IN";
//         }
//         else if (redir->type == TOKEN_REDIR_OUT)
//         {
//             type_str = ">";
//             type_name = "REDIR_OUT";
//         }
//         else if (redir->type == TOKEN_REDIR_APPEND)
//         {
//             type_str = ">>";
//             type_name = "REDIR_APPEND";
//         }
//         else if (redir->type == TOKEN_HEREDOC)
//         {
//             type_str = "<<";
//             type_name = "HEREDOC";
//         }
//         else
//         {
//             type_str = "?";
//             type_name = "UNKNOWN";
//         }
//         printf("    Redir: %s (%s) -> %s (expandable: %d)\n", type_str, type_name, redir->file, redir->quote_type);
//         redir = redir->next;
//     }
// }

// void print_parser(t_cmd *cmds)
// {
//     int cmd_num = 1;
//     int i;
//     while (cmds)
//     {
//         printf("Command %d:\n", cmd_num);
//         printf("  pipe_in: %d, pipe_out: %d\n", cmds->pipe_in, cmds->pipe_out);
//         // print argv
//         printf("  argv: ");
//         for (i = 0; cmds->argv && cmds->argv[i]; i++)
//         {
//             printf("\"%s...\" ", cmds->argv[i]);
//         }
//         printf("\n");
//         // print quote_types
//         printf("  quote_types: ");
//         for (i = 0; cmds->quote_types && cmds->argv && cmds->argv[i]; i++)
//         {
//             printf("%d ", cmds->quote_types[i]);
//         }
//         printf("\n");
//         // print redirections
//         if (cmds->redirs)
//             print_redirs(cmds->redirs);
//         printf("\n");
//         cmds = cmds->next;
//         cmd_num++;
//     }
// }

// void print_env(t_list *env)
// {
//     t_list *tmp = env;
//     while (tmp)
//     {
//         if (tmp->content) // Only print if value exists
//             printf("%s********\n", (char *)tmp->content);
//         tmp = tmp->next;
//     }
// }

int minishell_cycle(char *line, char **env)
{
	t_token *tokens;
	t_list *envc;
	t_cmd *cmds;
	tokens = tokenizer(line);
	if (!tokens)
		return (0);
	envc = env_copy(env);
	cmds = parser(tokens);
	if (!cmds)
	{
		tokens_clear(&tokens, free);
		ft_lstclear(&envc, free);
		cmds_cleaner(cmds);
		return (1);
	}
	tokens_clear(&tokens, free);
    // print_env(envc);
	expander(cmds, envc);
    //print_parser(cmds);
    //excute() Nour's Part يا رب يخلص
	cmds_cleaner(cmds);
	ft_lstclear(&envc, free);
	return (0);
}

int check_quotes_loop(char *line)
{
	if (check_quotes(line))
	{
		write(2, "Syntax error: unclosed quotes\n", 31);
		free(line);
		return (1);
	}
	return (0);
}

void readline_loop(t_term term, char **env)
{
	char *line;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			exit_handle(&term);
		if (!*line)
		{
			free(line);
			continue;
		}
		if (*line && not_all_space(line))
			add_history(line);
		if (check_quotes_loop(line))
			continue;
		if (minishell_cycle(line, env))
		{
			free(line);
			continue;
		}
		free(line);
	}
}

int main(int argc, char **argv, char **env)
{
	(void)argv;
	t_term term;

	check_arguments(argc);
	init_terminal(&term);
	signal_handle();
	readline_loop(term, env);
	return (0);
}
