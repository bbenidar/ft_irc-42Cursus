# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/26 14:23:33 by moudrib           #+#    #+#              #
#    Updated: 2024/01/06 22:35:15 by bbenidar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv
CC		=	c++
FLAGS	=	-Wall -Wextra -Werror -std=c++98
OBJDIR	=	Object_files/

SRC		=	src/main.cpp \
			src/utils/utils.cpp \
			src/irc_server/server.cpp \
			src/irc_server/multiplexing.cpp \
			src/Commands/authentication.cpp \
			src/irc_server/channel.cpp

HEADERS =	include/utils/utils.hpp \
			include/utils/colors.hpp \

G		= \x1B[32m \033[1m
W		= \x1B[0m

OBJ		= $(addprefix $(OBJDIR), $(SRC:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)

$(OBJDIR)%.o: %.cpp $(HEADERS)
	@mkdir -p $(@D)
	@$(CC) $(FLAGS) -c $< -o $@
	@echo "$(G)Compiling: $(W)$<"

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -rf $(NAME) $(OBJDIR)

re: fclean all