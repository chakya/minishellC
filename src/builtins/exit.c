/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <dphang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:02:40 by dphang            #+#    #+#             */
/*   Updated: 2024/04/16 16:51:13 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_numeric(char *str)
{
	int	i;

	i = 0;
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
			printf("exit: %s: numeric argument required\n", cmd[1]);
			return (2);
		}
		else if (cmd[1] && cmd[2])
		{
			printf("exit: too many arguments\n");
			return (1);
		}
		else if (cmd[1] && is_numeric(cmd[1]))
			return (ft_atoi(cmd[1]));
	}
	return (0);
}