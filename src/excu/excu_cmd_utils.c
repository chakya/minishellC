/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excu_cmd_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 11:20:00 by dphang            #+#    #+#             */
/*   Updated: 2024/04/23 11:26:02 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_executable(char *cmd)
{
	struct stat	buf;

	if (stat(cmd, &buf) == -1)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (127);
	}
	if (S_ISDIR(buf.st_mode))
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		return (126);
	}
	if (access(cmd, X_OK) == -1)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		return (126);
	}
	return (0);
}

int	envp_size(t_envp *envp)
{
	int		size;
	t_envp	*temp;

	size = 0;
	temp = envp;
	while (temp)
	{
		size++;
		temp = temp->next;
	}
	return (size);
}

char	**dup_envparr(t_envp *envp)
{
	t_envp	*temp;
	char	**envp_arr;
	int		i;

	i = 0;
	temp = envp;
	envp_arr = (char **)malloc((envp_size(envp) + 1) * sizeof(char *));
	if (!envp_arr)
		return (NULL);
	while (temp)
	{
		envp_arr[i] = ft_strdup(temp->content);
		temp = temp->next;
		i++;
	}
	envp_arr[i] = NULL;
	return (envp_arr);
}
