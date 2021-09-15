NAME = ircserv
SRC = main.cpp Server.cpp Client.cpp Socket.cpp network.cpp ServerErr.cpp Channel.cpp Command.cpp
INC = Server.hpp Client.hpp Socket.hpp Channel.hpp Command.hpp
CFLAGS = -std=c++98 -Wall -Wextra# -Werror

all: $(NAME)

$(NAME): $(SRC) $(INC)
	clang++ $(CFLAGS) $(SRC) -o $(NAME) -g

fclean:
	rm -rf $(NAME)

clean:
	rm -rf *.o

re: fclean clean all
