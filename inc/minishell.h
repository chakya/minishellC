/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:28:45 by dphang            #+#    #+#             */
/*   Updated: 2024/04/10 20:25:44 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#ifndef PATH_MAX
# define PATH_MAX 4096
#endif

#include "../lib/libft/libft.h"
#include <dirent.h>
#include <limits.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>

typedef struct s_envp
{
    char            *content;
    struct s_envp   *next;
}                   t_envp;

typedef struct	s_minishell
{
    t_envp	*envp;
    int		exit_status;
}			t_minishell;

typedef struct  s_signals
{
    struct sigaction    sigint_sa;
    struct sigaction    eof_sa;
    struct sigaction    sigquit_sa;
    int                 exit_sig;
}                       t_signals;

typedef enum s_type
{
	T_INVALID = 0,
	T_PIPE,
	T_CMD,
	T_ARG,
	T_COL,
	T_OB,
	T_CB,
	T_OR,
	T_AND,
	T_TRUNC,
	T_APPEND,
	T_INPUT,
	T_HEREDOC
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

typedef struct s_ast
{
	t_type			type;
	struct s_dls	*tokens;
	struct s_ast	**children;
}					t_ast;

extern t_signals g_sigs;

t_dls				*parse_token(char *input);
t_dls				*ft_dlsnew(char *content, t_type type);
void				ft_dlsadd_back(t_dls **lst, t_dls *new);
int					execute(t_dls *tokens, char **envp);
t_ast *parse_ast(t_dls *tokens);
int execute_ast(t_ast *ast, t_minishell **mnst, char** envp);

//	=========================   built-ins   ===================================
//	builtins_utils
int	ft_strcmp(const char *s1, const char *s2);
int	is_redir(char *str);
//  cd
int	cd(char **cmd, t_minishell **mnsh);
//	echo
int echo(char **cmd);
//	env
int	env(char **cmd, t_minishell *mnsh);
//  exit
int	mnsh_exit(void);
//  export
int export(char **cmd, t_minishell **mnsh);
//	pwd
int	pwd(void);
//  unset
int	unset(char **cmd, t_minishell **mnsh);
//  =========================   execution   ===================================
//  excu
void    excu(char **cmd, t_minishell **mnsh);
//  =========================   redirection   =================================
//  get_path
char	**get_path(t_envp *envp);
int excu_cmd(char **cmd, t_envp *envp);
//  =========================   initialization   ==============================
t_envp	*newenvp(char *var);
void	init_mnsh(char **envp, t_minishell **mnsh);
//  =========================   signals   ========================================
void    init_sigs(void);
//  =========================   free   ========================================
void	free_envp(t_envp **envp);
void    free_all(t_minishell **mnsh);

#endif
