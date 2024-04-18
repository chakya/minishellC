# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/05 22:26:43 by dphang            #+#    #+#              #
#    Updated: 2024/04/11 16:28:44 by cwijaya          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
LIBFT = lib/libft/libft.a
SRCS = \
		src/main.c \
		src/free.c \
		src/init.c \
		src/signals.c \
		src/builtins/cd.c \
		src/builtins/echo.c \
		src/builtins/env.c \
		src/builtins/exit.c \
		src/builtins/export.c \
		src/builtins/unset.c \
		src/builtins/pwd.c \
		src/builtins/builtins_utils.c \
		src/excu/excu.c \
		src/parser/parse_dollar.c \
		src/parser/parse_quotes.c \
		src/parser/parse_string.c \
		src/parser/parsing.c \
		src/lst/dls.c \
		src/excu/excu_cmd.c
#		src/lst/dls.c \
		src/parser/parsing.c \
		src/parser/split.c \
		src/pipe/mnsh_pipe.c
OBJS = $(SRCS:.c=.o)
RM = rm -rf

all : $(NAME)

$(NAME): $(OBJS)
	@${MAKE} -C lib/libft
	@${CC} ${CFLAGS} ${OBJS} ${LIBFT} -lreadline -o ${NAME} -g

.c.o:
	@${CC} ${CFLAGS} -c $< -o ${<:.c=.o} -g

clean:
	@$(RM) $(OBJS)
	@cd ./lib/libft && $(MAKE) clean

fclean: clean
	$(RM) $(NAME)
	@cd ./lib/libft && $(MAKE) fclean

re: fclean all
