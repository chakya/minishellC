/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mnsh_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <dphang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:02:40 by dphang            #+#    #+#             */
/*   Updated: 2024/04/04 16:59:29 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	unset(char **cmd, t_minishell **mnsh)
{
	t_envp *temp;
	int 	i;

	if (!cmd[1])
	{
		printf("unset: not enough arguments\n");
		return (1);
	}
	else if (cmd[1] && !(isalnum(cmd[1][0]) || cmd[1][0] == '_'))
	{
		printf("unset: '%s': invalid parameter name\n", cmd[1]);
		return (1);
	}
	i = 1;
	while (cmd[i])
	{
		temp = (*mnsh)->envp;
		while (ft_strncmp(temp->content, cmd[i], ft_strlen(cmd[i])) && temp->next)
		{
			temp = temp->next;
		}
		i++;
	}
	return (0);
}