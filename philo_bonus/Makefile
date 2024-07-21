# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/07 21:42:10 by bchanaa           #+#    #+#              #
#    Updated: 2024/05/16 18:34:56 by bchanaa          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -fsanitize=thread -g3

HEADERS = philo_bonus.h
SOURCES = main.c utils.c time_utils.c routines.c
OBJS := $(SOURCES:.c=.o)
OBJECTS = $(addprefix $(OBJECTS_DIR)/, $(OBJS))
OBJECTS_DIR = bin


NAME = philo_bonus

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
