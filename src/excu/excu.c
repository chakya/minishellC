/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 09:40:29 by dphang            #+#    #+#             */
/*   Updated: 2024/04/08 09:40:30 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int is_builtins(char **cmd)
{
    if (ft_strcmp(cmd[0], "cd") == 0
        || ft_strcmp(cmd[0], "echo") == 0
        || ft_strcmp(cmd[0], "env") == 0
        || ft_strcmp(cmd[0], "exit") == 0
        || ft_strcmp(cmd[0], "export") == 0
        || ft_strcmp(cmd[0], "pwd") == 0
        || ft_strcmp(cmd[0], "unset") == 0)
        return (1);
    return (0);
}

void    builtins(char **cmd, t_minishell **mnsh)
{
    if (ft_strcmp(cmd[0], "cd") == 0)
        cd(cmd, mnsh);
    else if (ft_strcmp(cmd[0], "echo") == 0)
        echo(cmd);
    else if (ft_strcmp(cmd[0], "env") == 0)
        env(cmd, *mnsh);
    else if (ft_strcmp(cmd[0], "exit") == 0)
        mnsh_exit(mnsh);
    else if (ft_strcmp(cmd[0], "export") == 0)
        export(cmd, mnsh);
    else if (ft_strcmp(cmd[0], "pwd") == 0)
        pwd();
    else if (ft_strcmp(cmd[0], "unset") == 0)
        unset(cmd, mnsh);
}

void    excu(char **cmd, t_minishell **mnsh)
{
    if (is_builtins(cmd))
        builtins(cmd, mnsh);
    //else find executable via PATH env var (?)
}