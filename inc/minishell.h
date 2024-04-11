/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <dphang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:28:45 by dphang            #+#    #+#             */
/*   Updated: 2024/04/05 09:52:14 by dphang           ###   ########.fr       */
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
int	mnsh_exit(t_minishell **mnsh);
//  export
int export(char **cmd, t_minishell **mnsh);
//	pwd
int	pwd(void);
//  unset
int	unset(char **cmd, t_minishell **mnsh);
//  =========================   execution   ===================================
//  excu
void    excu(char **cmd, t_minishell **mnsh);
//  =========================   initialization   ==============================
t_envp	*newenvp(char *var);
void	init_mnsh(char **envp, t_minishell **mnsh);
//  =========================   free   ========================================
void	free_envp(t_envp **envp);
void    free_all(t_minishell **mnsh);

#endif