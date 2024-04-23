/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excu_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:25:39 by dphang            #+#    #+#             */
/*   Updated: 2024/04/23 12:44:05 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	append_slash(char ***path)
{
	int		i;
	char	*dup_temp;

	i = 0;
	while ((*path)[i])
	{
		if ((*path)[i][ft_strlen((*path)[i]) - 1] != '/')
		{
			dup_temp = ft_strdup((*path)[i]);
			free((*path)[i]);
			(*path)[i] = ft_strjoin(dup_temp, "/");
			free(dup_temp);
		}
		i++;
	}
}

char	**get_path(t_envp *envp)
{
	char	**path;
	t_envp	*temp;

	temp = envp;
	while (temp && ft_strncmp(temp->content, "PATH=", 5) != 0)
	{
		temp = temp->next;
	}
	if (temp == NULL)
		return (NULL);
	path = ft_split((temp->content + 5), ':');
	append_slash(&path);
	return (path);
}

void	excu_pathcmd(int *exit_code, char **cmd, char **envp, char ***path)
{
	char	*excu_cmd;
	int		i;

	i = 0;
	excu_cmd = ft_strjoin((*path)[i], cmd[0]);
	while ((*path)[i] && execve(excu_cmd, cmd, envp) == -1)
	{
		free(excu_cmd);
		i++;
		if ((*path)[i])
			excu_cmd = ft_strjoin((*path)[i], cmd[0]);
		else
			excu_cmd = NULL;
	}
	if (excu_cmd)
		free(excu_cmd);
	else
	{
		*exit_code = 127;
		ft_putstr_fd(cmd[0], 2);
		ft_putstr_fd(": command not found\n", 2);
	}
	free_arr(path);
}

void	excu_cmd(char **cmd, t_minishell **mnsh)
{
	char	**envp;
	char	**path;
	int		exit_code;

	envp = dup_envparr((*mnsh)->envp);
	exit_code = 0;
	if (ft_strchr(cmd[0], '/'))
	{
		if (execve(cmd[0], cmd, envp) == -1)
			exit_code = is_executable(cmd[0]);
	}
	else if (cmd[0][0] != '#')
	{
		path = get_path((*mnsh)->envp);
		if (path == NULL)
			exit_code = is_executable(cmd[0]);
		else
			excu_pathcmd(&exit_code, cmd, envp, &path);
	}
	free_arr(&envp);
	exit(exit_code);
}
