/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excu_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:25:39 by dphang            #+#    #+#             */
/*   Updated: 2024/04/18 22:45:58 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**get_path(t_envp *envp)
{
	char	**path;
	t_envp	*temp;
    char    *dup_temp;
	int		i;

	i = 0;
    temp = envp;
	while (ft_strncmp(temp->content, "PATH=", 5) != 0)
	{
		temp = temp->next;
	}
    path = ft_split((temp->content + 5), ':');
	i = 0;
	while (path[i])
	{
		if (path[i][ft_strlen(path[i]) - 1] != '/')
		{
			dup_temp = ft_strdup(path[i]);
			free(path[i]);
			path[i] = ft_strjoin(dup_temp, "/");
			free(dup_temp);
		}
		i++;
	}
    return (path);
}

void excu_cmd(char **cmd, t_envp *envp)
{
    char	**path;
	char	*excu_cmd;
	int		i;

	path = get_path(envp);
	i = 0;
	excu_cmd = ft_strjoin(path[i], cmd[0]);
	while (path[i] && path[i+1] && execve(excu_cmd, cmd, NULL) == -1)
	{
		free(excu_cmd);
		i++;
		excu_cmd = ft_strjoin(path[i], cmd[0]);
	}
	if (excu_cmd)
		free(excu_cmd);
	else
		printf("%s: command not found\n", cmd[0]);
	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
    free(path);
}
