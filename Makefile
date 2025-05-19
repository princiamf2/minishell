NAME = minishell
SRCS =  builtin_unset.c env_add_remove.c exec_pipe_utils.c exec_extern.c \
		lexer_main.c main.c parse_main.c redirection.c token_adv.c cleanup_base.c \
		heredoc_file.c lexer_buffer.c parse_utils.c parse_andor.c \
		parse_subshell_collect.c parse_pipeline.c \
		signals.c exec_main.c utils.c env_utils.c parse_arg.c exec_path_utils.c \
		token_create.c lexer_collect.c lexer_chars.c exec_pipe.c token_base.c \
		cleanup_utils.c globbing_read.c lexer_op_1.c lexer_op_2.c \
		process_line.c utils_and_more.c local_vars.c lexer_expand_var.c token_utils.c token_expand.c \
		parse_command.c parse_subshell.c parse_subshell_utils.c parse_inter.c \
		parse_redirection.c exec_fork.c exec_subshell.c exec_redir_setup.c \
		heredoc_memo.c builtin_base.c builtin_export.c builtin_export_utils.c \
		builtin_export_arg.c builtin_dispatch.c builtin_exit.c builtin_utils.c env_init.c \
		env_update.c globbing.c globbing_matches.c param_expand.c expander.c arith.c \
		exec_line.c bultin_export_utils1.c complete_line.c expander_utils.c expander_utils2.c \
		utils2.c heredoc_file_utils.c


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

debug: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes --suppressions=ignore_readline_leaks.supp --log-file="leaks.log" ./minishell

.PHONY: all clean fclean re
