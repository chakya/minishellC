/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 09:40:29 by dphang            #+#    #+#             */
/*   Updated: 2024/04/23 19:46:22 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_builtins(char **cmd)
{
	if (ft_strcmp(cmd[0], "cd") == 0
		|| ft_strcmp(cmd[0], "echo") == 0
		|| ft_strcmp(cmd[0], "env") == 0
		|| ft_strcmp(cmd[0], "exit") == 0
		|| ft_strcmp(cmd[0], "export") == 0
		|| ft_strcmp(cmd[0], "pwd") == 0
		|| ft_strcmp(cmd[0], "unset") == 0)
		return (1);
	return (0);
}

int	builtins(char **cmd, t_minishell **mnsh)
{
	if (ft_strcmp(cmd[0], "cd") == 0)
		return (cd(cmd, mnsh));
	else if (ft_strcmp(cmd[0], "echo") == 0)
		return (echo(cmd));
	else if (ft_strcmp(cmd[0], "env") == 0)
		return (env(cmd, *mnsh));
	else if (ft_strcmp(cmd[0], "exit") == 0)
		return (mnsh_exit(cmd, mnsh));
	else if (ft_strcmp(cmd[0], "export") == 0)
		return (export(cmd, mnsh));
	else if (ft_strcmp(cmd[0], "pwd") == 0)
		return (pwd());
	else if (ft_strcmp(cmd[0], "unset") == 0)
		return (unset(cmd, mnsh));
	return (0);
}

char **split_cmd(char **cmd)
{
	int		i;
	int		j;
	char	**temp;
	char	**new_cmd;

	temp = ft_split(cmd[0], ' ');
	free(cmd[0]);
	i = 0;
	j = 0;
	while (temp[i])
		i++;
	while (cmd[j])
		j++;
	new_cmd = malloc(sizeof(char *) * (i + j + 1));
	if (new_cmd == NULL)
		return (NULL);
	i = -1;
	j = -1;
	while (temp[++i])
		new_cmd[i] = temp[i];
	while (cmd[++j + 1])
		new_cmd[i + j] = cmd[j + 1];
	free(cmd);
	free(temp);
	new_cmd[i + j] = NULL;
	return (new_cmd);
}


void	free_av(char **av)
{
	int	i;

	i = 0;
	while (av[i])
	{
		free(av[i]);
		i++;
	}
	free(av);
}

int	excu(char **cmd, t_minishell **mnsh)
{
	int exit_code;

	exit_code = 0;
	if (cmd[0])
	{
		if (ft_strchr(cmd[0], ' '))
			cmd = split_cmd(cmd);
		if (is_builtins(cmd))
			exit_code = builtins(cmd, mnsh);
		else
			excu_cmd(cmd, mnsh);
	}
	free_av(cmd);
	return (exit_code);
}
