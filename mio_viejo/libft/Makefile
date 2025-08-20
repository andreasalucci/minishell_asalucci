# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: inbauman <inbauman@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/21 10:09:46 by inbauman          #+#    #+#              #
#    Updated: 2025/02/19 15:47:17 by inbauman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

LIB_OBJECTS = ft_isalpha.o ft_isdigit.o ft_isalnum.o ft_isascii.o ft_isprint.o ft_memset.o ft_strlen.o ft_bzero.o ft_memcpy.o ft_memmove.o \
ft_strlcpy.o ft_toupper.o ft_strlcat.o ft_tolower.o ft_strchr.o ft_strrchr.o ft_strncmp.o ft_memchr.o ft_memcmp.o ft_atoi.o ft_strnstr.o ft_calloc.o \
ft_strdup.o ft_substr.o ft_strjoin.o ft_strtrim.o ft_itoa.o ft_strmapi.o ft_striteri.o ft_putchar_fd.o ft_putstr_fd.o ft_putendl_fd.o ft_putnbr_fd.o ft_split.o \
get_next_line.o ft_printf.o ft_printf_funciones.o

CC = gcc
CFLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(LIB_OBJECTS): %.o: %.c libft.h
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(LIB_OBJECTS)
	ar rcs $(NAME) $(LIB_OBJECTS)

clean:
	rm -f $(LIB_OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re

