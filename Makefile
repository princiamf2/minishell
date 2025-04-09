NAME = minishell
SRCS = builting.c builting2.c env.c execute_pipe.c execute.c \
		lexer.c main.c parsing.c redirections.c token.c utils.c \
		heredoc_file_utils.c utils_lex5.c utils_parser.c command_assembly.c \
		subshell_exec_utils.c pipeline_and_glob_utils.c utils2.c \
		utils3.c cmd_execution.c cmd_management.c utils_env.c utils_env2.c utils_exec.c \
		utils_lex1.c utils_lex2.c utils_lex3.c utils_pipe.c export_utils.c utils_token.c \
		arg_expansion_utils.c heredoc_buffer_utils.c utils_lex4.c utils_token_expand.c exit.c
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
