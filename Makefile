# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: moudrib <moudrib@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/26 14:23:33 by moudrib           #+#    #+#              #
#    Updated: 2024/01/07 16:29:55 by moudrib          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv
CC		=	c++
FLAGS	=	-Wall -Wextra -Werror -std=c++98 -fsanitize=address
OBJDIR	=	Object_files/

SRC		=	src/main.cpp \
			src/utils/utils.cpp \
			src/irc_server/server.cpp \
			src/irc_server/multiplexing.cpp \
			src/Commands/Pass.cpp \
			src/Commands/Nick.cpp \
			src/Commands/commands.cpp \
			src/Commands/authentication.cpp

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