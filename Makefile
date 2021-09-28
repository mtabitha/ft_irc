.PHONY: all clean fclean re

CMD_P = ./Commands/

CMD_SRCS = Command.cpp \
			Join.cpp \
			Mode.cpp \
			Nick.cpp \
			Pass.cpp \
			Quit.cpp \
			User.cpp \
			Invite.cpp \
			Kick.cpp \
			Names.cpp \
			Part.cpp \
			Privmsg.cpp \
			Topic.cpp

OTHER_SRCS = Channel.cpp \
			Client.cpp \
			Server.cpp \
			ServerErr.cpp \
			Socket.cpp \
			main.cpp \
			network.cpp 

NAME = ircserv
CC = clang++
CFLAGS = -g -Wall -Wextra -Werror 

OBJS = $(SRCS:.cpp=.o)

SRCS = $(OTHER_SRCS) $(P_CMD_SRCS)

P_CMD_SRCS =	$(addprefix $(CMD_P), $(CMD_SRCS))

all :$(NAME)

$(NAME) : $(OBJS)
		$(CC) $(CFLAGS) $(OBJS) -o $(NAME) 

%.o: %.cpp 
	$(CC) $(CFLAGS) -c $< -o $@ 

clean : 
		@rm -f $(OBJS)

fclean : clean
		@rm -f $(NAME)

re : fclean all
