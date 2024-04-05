/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:28:45 by dphang            #+#    #+#             */
/*   Updated: 2024/04/05 16:08:50 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <sys/ioctl.h>
# include <sys/resource.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>

typedef struct s_minishell
{
	char			**envp;
	int				exit_status;
}					t_minishell;

typedef enum s_type
{
	T_INVALID = 0,
	T_AND,
	T_OR,
	T_L,
	T_R,
	T_DL,
	T_DR,
	T_PIPE,
	T_COL,
	T_OPEN_B,
	T_CLOSE_B,
	T_CMD,
	T_ARG
}					t_type;

typedef struct s_dls
{
	char			*content;
	t_type			type;
	struct s_dls	*prev;
	struct s_dls	*next;
}					t_dls;

typedef struct s_btree
{
	char			*content;
	struct s_btree	*left;
	struct s_btree	*right;
}					t_btree;

//	built-ins
int					echo(char **cmd);
int					env(char **cmd, char **envp);
int					pwd(void);
t_dls				*parse_token(char *input);
t_dls				*ft_dlsnew(char *content, t_type type);
void				ft_dlsadd_back(t_dls **lst, t_dls *new);
int					execute(t_dls *tokens, char **envp);

#endif
