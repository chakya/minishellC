# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dphang <dphang@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/05 22:26:43 by dphang            #+#    #+#              #
#    Updated: 2024/04/05 22:34:40 by dphang           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = lib/libft/libft.a
SRCS = \
		src/main.c \
		src/builtins/mnsh_cmd.c \
		src/lst/dls.c \
		src/parser/parsing.c \
		src/parser/split.c \
		src/pipe/mnsh_pipe.c
OBJS = $(SRCS:.c=.o)
RM = rm -rf

all : $(NAME)

$(NAME): $(OBJS)
	@${MAKE} -C lib/libft
	@${CC} ${CFLAGS} ${OBJS} ${LIBFT} -o ${NAME}

.c.o:
	@${CC} ${CFLAGS} -c $< -o ${<:.c=.o}

clean:
	@$(RM) $(OBJS)
	@cd ./lib/libft && $(MAKE) clean

fclean: clean
	$(RM) $(NAME)
	@cd ./lib/libft && $(MAKE) fclean

re: fclean all