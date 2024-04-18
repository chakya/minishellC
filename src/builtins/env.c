/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:02:40 by dphang            #+#    #+#             */
/*   Updated: 2024/04/10 19:54:13 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	env(char **cmd, t_minishell *mnsh)
{
	t_envp	*temp;

	temp = mnsh->envp;
	if (cmd[1] && !is_redir(cmd[1]))
	{
		printf("env: '%s': No such file or directory\n", cmd[1]);
		return (1);
	}
	else
	{
		while (temp)
		{
			printf("%s\n", temp->content);
			temp = temp->next;
		}
	}
	return (0);
}
