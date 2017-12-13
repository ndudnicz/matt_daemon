# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmanet <tmanet@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/09 10:10:37 by tmanet            #+#    #+#              #
#    Updated: 2017/12/07 19:07:49 by tmanet           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = Matt_daemon

CXX = g++

FLAGS = -Wall -Wextra -Werror

SRC =	main.cpp \
		Tintin_reporter.cpp \
		Connection.cpp \
		Server.cpp

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(FLAGS) -o $@ $(OBJ)
	@echo "COMPILATION de $(NAME)"

%.o: %.cpp
	@$(CXX) $(FLAGS) -o $@ -c $<
	@echo "compilation de $<"

clean:
	@/bin/rm -f $(OBJ)
	@echo "suppression des objets"

fclean: clean
	@/bin/rm -f $(NAME)
	@echo "suppression de $(NAME)"

re: fclean all

.PHONY: all clean fclean re
