NAME = ircserver

SRCSDIR = src
INCDIR = include

SRCS = $(SRCSDIR)/main.cpp \
		$(SRCSDIR)/server.cpp \
		$(SRCSDIR)/command.cpp \
		$(SRCSDIR)/channel.cpp \
		$(SRCSDIR)/client.cpp \
		$(SRCSDIR)/join.cpp \
		$(SRCSDIR)/kick.cpp \
		$(SRCSDIR)/mode.cpp \
		$(SRCSDIR)/utils.cpp \
		$(SRCSDIR)/topic.cpp \
		
OBJS = $(SRCS:.cpp=.o)

CC = g++
CFLAGS = -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean

re: fclean all

.PHONY: all clean fclean re