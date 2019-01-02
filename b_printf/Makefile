# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qpeng <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/09/18 23:44:47 by qpeng             #+#    #+#              #
#    Updated: 2018/09/24 00:31:02 by qpeng            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

FLAGS =  -Wall -Werror -Wextra 

SRC = ft_*.c

OBJS = $(SRC:.c=.o)

all: $(NAME) 

$(NAME): $(OBJS)
	@ar rc $(NAME) $(OBJS)
	@ranlib $(NAME)
	@echo "\033[0;32m ✓ Created $(NAME)"

$(OBJS): $(CFILES) 
	@gcc $(FLAGS) -c $(SRC)

clean:
	@/bin/rm -rf *.o
	@echo "\033[0;34m ✓ Object files Removed"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[0;31m ✓ Removed $(NAME)"

re: fclean all

.PHONY: clean fclean all re
