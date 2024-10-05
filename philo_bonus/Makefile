# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/07 21:42:10 by bchanaa           #+#    #+#              #
#    Updated: 2024/10/05 18:25:17 by bchanaa          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address

HEADERS = philo_bonus.h
SOURCES = main.c utils.c time_utils.c routines.c child.c init.c threads.c
OBJS := $(SOURCES:.c=.o)
OBJECTS = $(addprefix $(OBJECTS_DIR)/, $(OBJS))
OBJECTS_DIR = bin


NAME = philo_bonus

all: $(NAME)

$(NAME): $(OBJECTS)
	$(CC) -pthread $(CFLAGS) $(OBJECTS) -o $@

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
