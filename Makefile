.PHONY: all clean fclean re 

CC = clang++
CFLAGS = -g -Wall -Wextra -Werror -I.
NAME = ircserv

OBJS = $(SRC:.cpp=.o)

SRC = Channel.cpp \
		Client.cpp \
		Command.cpp \
		Server.cpp \
		ServerErr.cpp \
		Socket.cpp \
		main.cpp \
		network.cpp


INC = Channel.hpp \
		Client.hpp \
		Command.hpp \
		Server.hpp \
		Socket.hpp \
		network.hpp

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o : %.cpp $(INC)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)
