/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:02:40 by dphang            #+#    #+#             */
/*   Updated: 2024/04/23 11:16:18 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	rm_envp(t_minishell **mnsh, t_envp **to_rm)
{
	t_envp	*temp;

	if (ft_strcmp((*mnsh)->envp->content, (*to_rm)->content) == 0)
	{
		temp = (*mnsh)->envp;
		(*mnsh)->envp = (*mnsh)->envp->next;
		free(temp->content);
		free(temp);
	}
	else
	{
		temp = (*mnsh)->envp;
		while (temp && !(ft_strncmp(temp->next->content, (*to_rm)->content,
					ft_strlen((*to_rm)->content)) == 0))
		{
			temp = temp->next;
		}
		temp->next = (*to_rm)->next;
		free((*to_rm)->content);
		free(*to_rm);
	}
}

int	unset(char **cmd, t_minishell **mnsh)
{
	t_envp	*temp;
	int		i;

	i = 1;
	while (cmd[i])
	{
		temp = (*mnsh)->envp;
		while (temp && (ft_strncmp(temp->content, cmd[i],
					ft_strlen(cmd[i])) != 0))
		{
			temp = temp->next;
		}
		if (temp)
			rm_envp(mnsh, &temp);
		i++;
	}
	return (0);
}
