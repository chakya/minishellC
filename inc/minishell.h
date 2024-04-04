/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <dphang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 23:28:45 by dphang            #+#    #+#             */
/*   Updated: 2024/04/04 13:12:02 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

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


typedef struct	s_minishell
{
    char	**envp;
    int		exit_status;
}			t_minishell;

//	built-ins
int echo(char **cmd);
int	env(char **cmd, char **envp);
int	pwd(void);

#endif
