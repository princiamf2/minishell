NAME = minishell
SRCS = builting.c builting2.c env.c execute_pipe.c execute.c \
		lexer.c main.c parsing.c redirections.c token.c utils.c \
		utils2.c utils3.c utils4.c utils5.c utils6.c utils7.c utils8.c \
		utils9.c utils10.c utils11.c utils_env.c utils_env2.c utils_exec.c \
		utils_lex1.c utils_lex2.c utils_lex3.c utils_pipe.c utils12.c utils_token.c \
		utils13.c utils14.c utils_lex4.c utils_token_expand.c
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LIBFT_DIR = ./libft
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -lreadline

all: $(NAME)

$(NAME): $(LIBFT) $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -L$(LIBFT_DIR) -lft $(LIBS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
