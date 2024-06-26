/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:02:40 by dphang            #+#    #+#             */
/*   Updated: 2024/04/24 15:51:47 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	mnsh_exit(char **cmd, t_minishell **mnsh)
{
	(*mnsh)->exit_sig = 1;
	printf("exit\n");
	if (cmd[0])
	{
		if (cmd[1] && !is_numeric(cmd[1]))
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			return (2);
		}
		else if (cmd[1] && cmd[2])
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			return (1);
		}
		else if (cmd[1] && is_numeric(cmd[1]))
			return (ft_atoi(cmd[1]));
	}
	return (0);
}

void	exit_child(t_minishell **mnsh, int exit_status)
{
	if (WIFEXITED(exit_status))
		(*mnsh)->exit_code = WEXITSTATUS(exit_status);
	if (WIFSIGNALED(exit_status))
	{
		if (WTERMSIG(exit_status) == SIGINT)
		{
			g_sig_received = 1;
			printf("\n");
		}
		else if (WTERMSIG(exit_status) == SIGQUIT)
		{
			(*mnsh)->exit_code = 131;
			printf("\n");
		}
	}
}
