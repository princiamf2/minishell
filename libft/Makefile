# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mm-furi <marvin@42lausanne.ch>             +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/03 10:01:40 by mm-furi           #+#    #+#              #
#    Updated: 2024/10/03 17:06:05 by mm-furi          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
NAME = libft.a
FLAG = -Wall -Wextra -Werror
SRCS = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c ft_isascii.c  ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c ft_memcmp.c \
ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c ft_putstr_fd.c ft_putnbr_fd.c ft_split.c ft_strchr.c ft_strdup.c ft_striteri.c \
ft_strjoin.c ft_strlcat.c ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c ft_toupper.c 
SRCB = ft_lstadd_front_bonus.c ft_lstadd_back_bonus.c ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c \
ft_lstnew_bonus.c ft_lstsize_bonus.c
LIB = ranlib
AR = ar rcs
OBJS = $(SRCS:.c=.o)
OBJB = $(SRCB:.c=.o)

all : $(NAME)

.c.o :
	$(CC) $(FLAG) -c $< -o $(<:.c=.o)

$(OBJS) : $(INCS)
$(OBJB) : $(INCB)

$(NAME) : $(OBJS)
	$(AR) $(NAME) $(OBJS)
	$(LIB) $(NAME)

bonus : $(OBJB)
	$(AR) $(NAME) $(OBJB)
	$(LIB) $(NAME)

clean :
	rm -f $(OBJS) $(OBJB)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY : all bonus clean fclean re .c.o

