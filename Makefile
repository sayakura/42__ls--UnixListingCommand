# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qpeng <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/01/02 18:16:06 by qpeng             #+#    #+#              #
#    Updated: 2019/01/02 19:21:28 by qpeng            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_ls
CC = gcc
CC_FLAGS = -Wall -Werror -Wextra -ffast-math -fno-signed-zeros -fno-trapping-math

LFT_PATH = ./libft/
SRC_PATH = ./src/
INC_PATH = ./include/ $(LFT_PATH)/include

SRC = $(addprefix $(SRC_PATH),$(SRC_NAME))
INC = $(addprefix -I,$(INC_PATH))

OBJ_NAME = $(SRC_NAME:.c=.o)
SRC_NAME = *.c

all: $(NAME)

$(NAME): $(OBJ)
	@make -C $(LFT_PATH)
	@gcc -c srcs/*.c -I includes/
	@$(CC) $(CC_FLAGS) *.o -o ft_ls -L$(LFT_PATH) -lft
	@rm -rf *.o
	@printf 'Building...done 🛠\n'

clean:
	@rm -rf *.o
	@echo "Cleaning...done 🚿"

fclean: clean
	@rm -f $(NAME)

re: fclean all
	@echo "==========================="

.PHONY: all, $(NAME), clean, fclean, re
