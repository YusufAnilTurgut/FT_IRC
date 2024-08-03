SRCS = Client.cpp  Channel.cpp Server.cpp utils.cpp process.cpp main.cpp  ./commands/cap.cpp  ./commands/join.cpp  ./commands/notice.cpp \
./commands/nick.cpp  ./commands/pass.cpp  ./commands/ping.cpp  ./commands/privmsg.cpp  ./commands/quit.cpp \
./commands/user.cpp  ./commands/mode.cpp ./commands/who.cpp ./commands/names.cpp ./commands/part.cpp ./commands/command_utils.cpp \
./commands/kick.cpp ./commands/topic.cpp

NAME = ircserv

OBJS = $(SRCS:.cpp=.o)

CC = c++

RM = rm -rf

FLAGS = -Wall -Wextra -Werror -std=c++98

all : $(NAME)

$(NAME) :	$(OBJS)
				$(CC) $(FLAGS) $(OBJS) -o $(NAME)

.cpp.o :
	@$(CC) $(FLAGS) -c $< -o $(<:.cpp=.o)

clean :
				$(RM) $(OBJS)

fclean :		clean
				$(RM) $(NAME)

re : fclean all

.PHONY = all clean fclean re