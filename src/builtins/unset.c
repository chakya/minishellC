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

void	rm_envp(t_minishell **mnsh, t_envp **to_rm)
{
	t_envp	*temp;

	temp = (*mnsh)->envp;
	while (temp && !(ft_strncmp(temp->next->content, (*to_rm)->content, ft_strlen((*to_rm)->content)) == 0))
	{
		temp = temp->next;
	}
	temp->next = (*to_rm)->next;
	free((*to_rm)->content);
	free(*to_rm);
}

int	unset(char **cmd, t_minishell **mnsh)
{
	t_envp *temp;
	int 	i;

	i = 1;
	if (!cmd[i])
	{
		printf("unset: not enough arguments\n");
		return (1);
	}
	while (cmd[i])
	{
		if (cmd[i] && !(isalnum(cmd[i][0]) || cmd[i][0] == '_'))
		{
			printf("unset: '%s': invalid parameter name\n", cmd[1]);
			return (1);
		}
		temp = (*mnsh)->envp;
		while (temp && (ft_strncmp(temp->content, cmd[i], ft_strlen(cmd[i])) != 0 ))
		{
			temp = temp->next;
		}
		rm_envp(mnsh, &temp);
		i++;
	}
	return (0);
}