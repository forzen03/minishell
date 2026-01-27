NAME = minishell

SRCS = ./qadah/main.c ./qadah/utliltes1.c ./qadah/signal_handle.c ./qadah/lexer.c ./qadah/lexer1.c ./qadah/lexer2.c ./qadah/lexer3.c \
		./qadah/parser_check_syntax.c ./qadah/parser_redirections.c ./qadah/parser_utliltes.c \
		./qadah/parser_utliltes2.c ./qadah/parser.c \
		./noor/builtin_execution.c \
		./noor/builtins.c \
		./noor/fill.c \
		./noor/pipe_handling.c \
		./noor/preparation.c \
		./noor/preparation_utils.c \

OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a

READLINE = -lreadline

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_DIR)

%.o: %.c minishell.h
	$(CC) $(CFLAGS) -I./libft -c $< -o $@

all: $(NAME)

clean:
	make clean -C $(LIBFT_DIR)
	rm -f $(OBJS)

fclean: clean
	make fclean -C $(LIBFT_DIR)
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
