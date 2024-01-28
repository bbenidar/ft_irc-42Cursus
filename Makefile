# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bbenidar <bbenidar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/26 14:23:33 by moudrib           #+#    #+#              #
#    Updated: 2024/01/27 21:22:41 by bbenidar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv
BOT		=	emet
CC		=	c++
FLAGS	=	-Wall -Wextra -Werror -std=c++98 -fsanitize=address
OBJDIR	=	Object_files/
B_OBJDIR	=	Object_files_bot/
ADDED_FILES	=	.vscode

SRC		=	src/main.cpp \
			src/utils/utils.cpp \
			src/utils/replies.cpp \
			src/Commands/Pass.cpp \
			src/Commands/Nick.cpp \
			src/Commands/User.cpp \
			src/Commands/Join.cpp \
			src/Commands/Kick.cpp \
			src/Commands/Mode.cpp \
			src/Commands/Topic.cpp \
			src/Commands/Notice.cpp \
			src/Commands/Invite.cpp \
			src/Commands/Privmsg.cpp \
			src/Commands/commands.cpp \
			src/Channels/channels.cpp \
			src/irc_server/server.cpp \
			src/irc_server/multiplexing.cpp

B_SRC	=	bot/main.cpp \
			bot/Bot.cpp \

HEADERS =	include/utils/utils.hpp \
			include/utils/colors.hpp \
			include/irc_server/server.hpp \
			include/Channels/channels.hpp \

G		= \x1B[32m \033[1m
W		= \x1B[0m

OBJ		= $(addprefix $(OBJDIR), $(SRC:.cpp=.o))

B_OBJ	= $(addprefix $(B_OBJDIR), $(B_SRC:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)


$(OBJDIR)%.o: %.cpp $(HEADERS)
	@mkdir -p $(@D)
	@$(CC) $(FLAGS) -c $< -o $@
	@echo "$(G)Compiling: $(W)$<"

bonus: $(BOT)

$(BOT): $(B_OBJ)
	@$(CC) $(FLAGS) $(B_OBJ) -o $(BOT)

$(B_OBJDIR)%.o: %.cpp bot/Bot.hpp
	@mkdir -p $(@D)
	@$(CC) $(FLAGS) -c $< -o $@
	@echo "$(G)Compiling: $(W)$<"

clean:
	@rm -rf $(OBJDIR) $(B_OBJDIR)

fclean: clean
	@rm -rf $(NAME) $(BOT) $(OBJDIR) $(B_OBJDIR) $(ADDED_FILES)

re: fclean all