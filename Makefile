# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/07 21:42:10 by bchanaa           #+#    #+#              #
#    Updated: 2024/05/15 22:24:53 by bchanaa          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=address -g3

HEADERS = philo.h
SOURCES = main.c init.c utils.c time_utils.c routines.c
OBJS := $(SOURCES:.c=.o)
OBJECTS = $(addprefix $(OBJECTS_DIR)/, $(OBJS))
OBJECTS_DIR = bin


NAME = philo

all: $(NAME)

address: $(OBJECTS)
	$(CC) $(CFLAGS) -fsanitize=address $(OBJECTS) -o $(NAME)

thread: $(OBJECTS)
	$(CC) $(CFLAGS) -fsanitize=thread $(OBJECTS) -o $(NAME)

$(NAME): $(OBJECTS)
	cc $(CFLAGS) $(OBJECTS) -o $@

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
