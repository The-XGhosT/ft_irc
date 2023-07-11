# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: anaji-el <anaji-el@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/07/09 03:41:14 by anaji-el          #+#    #+#              #
#    Updated: 2023/07/09 03:46:28 by anaji-el         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g
RM = rm -rf

COMMAND = command/Info.cpp  command/Join.cpp  command/Mode.cpp command/Pass.cpp command/Topic.cpp command/User.cpp \
		command/Utils.cpp command/Nick.cpp command/Invite.cpp command/Kick.cpp command/Prvmsg.cpp command/Part.cpp
BONUS = bonus/Bot.cpp
SRCS =  Replay.cpp main.cpp Cmds.cpp Server.cpp Client.cpp  Channels.cpp $(COMMAND) $(BONUS)
INCL = Replay.hpp Cmds.hpp Server.hpp Client.hpp  Channels.hpp bonus/Bot.hpp

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o:%.cpp $(INCL)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all
