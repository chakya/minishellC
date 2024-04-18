/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 09:40:29 by dphang            #+#    #+#             */
/*   Updated: 2024/04/18 22:05:49 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_builtins(char **cmd)
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

void	builtins(char **cmd, t_minishell **mnsh)
{
	if (ft_strcmp(cmd[0], "cd") == 0)
		(*mnsh)->exit_code = cd(cmd, mnsh);
	else if (ft_strcmp(cmd[0], "echo") == 0)
		(*mnsh)->exit_code = echo(cmd);
	else if (ft_strcmp(cmd[0], "env") == 0)
		(*mnsh)->exit_code = env(cmd, *mnsh);
	else if (ft_strcmp(cmd[0], "exit") == 0)
		(*mnsh)->exit_code = mnsh_exit(cmd, mnsh);
	else if (ft_strcmp(cmd[0], "export") == 0)
		(*mnsh)->exit_code = export(cmd, mnsh);
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		(*mnsh)->exit_code = pwd();
	else if (ft_strcmp(cmd[0], "unset") == 0)
		(*mnsh)->exit_code = unset(cmd, mnsh);
}

void	excu(char **cmd, t_minishell **mnsh)
{
	if (cmd[0])
	{
		if (is_builtins(cmd))
			builtins(cmd, mnsh);
		else
			excu_cmd(cmd, (*mnsh)->envp);
	}
}
