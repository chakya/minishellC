# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dphang <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/05 22:26:43 by dphang            #+#    #+#              #
#    Updated: 2024/04/23 11:43:18 by dphang           ###   ########.fr        #
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
		src/builtins/export_sort.c \
		src/builtins/export_utils.c \
		src/builtins/export.c \
		src/builtins/unset.c \
		src/builtins/pwd.c \
		src/builtins/builtins_utils.c \
		src/excu/excu_cmd_utils.c \
		src/excu/excu_cmd.c \
		src/excu/excu.c \
		src/parser/expand_dollar_utils.c \
		src/parser/expand_dollar.c \
		src/parser/parse_quotes.c \
		src/parser/parse_string.c \
		src/parser/parsing.c \
		src/parser/parser_utils.c \
		src/parser/token.c \
		src/parser/exe_token.c \
		src/parser/ast.c \
		src/parser/pipe.c \
		src/lst/dls.c
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
