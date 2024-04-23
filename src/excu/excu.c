/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 09:40:29 by dphang            #+#    #+#             */
/*   Updated: 2024/04/23 11:25:28 by dphang           ###   ########.fr       */
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

int	builtins(char **cmd, t_minishell **mnsh)
{
	if (ft_strcmp(cmd[0], "cd") == 0)
		return (cd(cmd, mnsh));
	else if (ft_strcmp(cmd[0], "echo") == 0)
		return (echo(cmd));
	else if (ft_strcmp(cmd[0], "env") == 0)
		return (env(cmd, *mnsh));
	else if (ft_strcmp(cmd[0], "exit") == 0)
		return (mnsh_exit(cmd, mnsh));
	else if (ft_strcmp(cmd[0], "export") == 0)
		return (export(cmd, mnsh));
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		return (pwd());
	else if (ft_strcmp(cmd[0], "unset") == 0)
		return (unset(cmd, mnsh));
	return (0);
}

int	excu(char **cmd, t_minishell **mnsh)
{
	if (cmd[0])
	{
		if (is_builtins(cmd))
			return (builtins(cmd, mnsh));
		else
			excu_cmd(cmd, mnsh);
	}
	return (0);
}
