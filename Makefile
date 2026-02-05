NAME = minishell

# Source files from qadah folder
QADAH_SRCS = qadah/main.c qadah/utliltes1.c qadah/signal_handle.c \
	qadah/lexer.c qadah/lexer1.c qadah/lexer2.c qadah/lexer3.c \
	qadah/parser_check_syntax.c qadah/parser_redirections.c \
	qadah/parser_utliltes.c qadah/parser_utliltes2.c qadah/parser.c \
	qadah/expander.c qadah/expander1.c qadah/exapnder2.c qadah/lexer4.c \
	qadah/lexer5.c	qadah/main_utlis.c

# Source files from noor folder
NOOR_SRCS = noor/builtin_execution.c noor/builtins.c noor/builtin_cd.c \
	noor/builtin_exit.c noor/builtin_export.c noor/builtin_pwd.c \
	noor/builtins_utils_2.c noor/env_utils.c noor/execution.c \
	noor/execution_utils.c noor/heredoc.c noor/heredoc_utils.c \
	noor/heredoc_utils_2.c noor/path_utils.c noor/pipe_handling.c \
	noor/preparation_utils.c noor/preparation.c noor/redirections.c

SRCS = $(QADAH_SRCS) $(NOOR_SRCS)

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror -I. -I./libft

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

READLINE = -lreadline

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJS)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
