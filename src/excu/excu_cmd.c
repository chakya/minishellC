/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excu_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:25:39 by dphang            #+#    #+#             */
/*   Updated: 2024/04/21 17:52:32 by cwijaya          ###   ########.fr       */
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

int	is_executable(char *cmd)
{
	struct stat	buf;

	if (stat(cmd, &buf) == -1)
	{
		printf("%s: No such file or directory\n", cmd);
		return (127);
	}
	if (S_ISDIR(buf.st_mode))
	{
		printf("%s: Is a directory\n", cmd);
		return (126);
	}
	if (access(cmd, X_OK) == -1)
	{
		printf("%s: Permission denied\n", cmd);
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

void	free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

void excu_cmd(char **cmd, t_minishell **mnsh)
{
    char	**path;
	char	*excu_cmd;
	int		exit_code;
	int		i;
	char	**envp;

	envp = dup_envparr((*mnsh)->envp);
	exit_code = 0;
	//to run scripts #!/bin/bash is needed (why?)
	if (ft_strchr(cmd[0], '/'))
	{
		if (execve(cmd[0], cmd, envp) == -1)
			exit_code = is_executable(cmd[0]);
	}
	else
	{
		path = get_path((*mnsh)->envp);
		i = 0;
		excu_cmd = ft_strjoin(path[i], cmd[0]);
		while (path[i] && execve(excu_cmd, cmd, envp) == -1)
		{
			free(excu_cmd);
			i++;
			if (path[i])
				excu_cmd = ft_strjoin(path[i], cmd[0]);
			else
				excu_cmd = NULL;
		}
		if (excu_cmd)
			free(excu_cmd);
		else
		{
			exit_code = 127;
			printf("%s: command not found\n", cmd[0]);
		}
		i = 0;
		while (path[i])
		{
			free(path[i]);
			i++;
		}
		free(path);
		free_arr(envp);
	}
	exit(exit_code);
}
