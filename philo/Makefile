# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/07 21:42:10 by bchanaa           #+#    #+#              #
#    Updated: 2024/10/02 15:09:41 by bchanaa          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror #-fsanitize=address -g3

HEADERS = philo.h
SOURCES = main.c init.c utils.c time_utils.c philosopher.c routines.c utils2.c libft_funcs.c cleanup.c
OBJS := $(SOURCES:.c=.o)
OBJECTS = $(addprefix $(OBJECTS_DIR)/, $(OBJS))
OBJECTS_DIR = bin


NAME = philo

all: $(NAME)

$(NAME): $(OBJECTS)
	cc -pthread $(CFLAGS) $(OBJECTS) -o $@

$(OBJECTS_DIR)/%.o: %.c $(HEADERS) | $(OBJECTS_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJECTS_DIR):
	mkdir -p $(OBJECTS_DIR)

clean:
	rm -f $(OBJECTS)
fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: re fclean clean
