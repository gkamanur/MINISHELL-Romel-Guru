# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/29 14:01:48 by gkamanur          #+#    #+#              #
#    Updated: 2025/08/07 12:40:31 by gkamanur         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = minishell

CC = cc -fsanitize=address

FLAGS =  #-Werror -Wall -Wextra

RM = rm -rf

include source.list
	  
OBJS = $(SRC:.c=.o)

HDR = includes/minishell.h\
	  includes/colors.h
	  
all : $(NAME)

$(NAME) : $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -lreadline -o $(NAME)
	
%.o: %.c $(HDR)
	$(CC) $(FLAGS) -c $< -o $@

clean : 
	$(RM) $(OBJS)
	
fclean : clean
		$(RM) $(NAME)
		
re : fclean all
	$(RM) $(OBJS)

## valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind-out.txt ./minishell
