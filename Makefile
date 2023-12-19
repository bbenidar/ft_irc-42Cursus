NAME	=	ft_irc
CC		=	c++
FLAGS	=	-Wall -Wextra -Werror -std=c++98
OBJDIR	=	Object_files/
SRC		=	src/main.cpp \
			src/irc_server/server.cpp

HEADERS =	include/utils/colors.hpp

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