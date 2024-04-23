/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:02:40 by dphang            #+#    #+#             */
/*   Updated: 2024/04/23 11:16:24 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	echo(char **cmd)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (cmd[i] && ft_strcmp(cmd[i], "-n") == 0)
	{
		flag = 1;
		i++;
	}
	while (cmd[i] && !is_redir(cmd[i]))
	{
		if ((i > 1 && flag == 0) || i > 2)
			printf(" %s", cmd[i]);
		else
			printf("%s", cmd[i]);
		i++;
	}
	if (flag == 0)
		printf("\n");
	return (0);
}
