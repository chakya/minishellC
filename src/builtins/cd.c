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

char	*ft_strpathdup(const char *path, const char *value)
{
	int		i;
	int		j;
	int		len;
	char	*dup;

	i = 0;
	j = 0;
	len = ft_strlen(path) + ft_strlen(value);
	dup = (char *)malloc((len + 1) * sizeof(char));
	if (!dup)
		return (((void *) 0));
	while (path[i])
	{
		dup[i] = path[i];
		i++;
	}
	while (value[j])
	{
		dup[i] = value[j++];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

void	update_wd(t_minishell **mnsh)
{
	t_envp	*wd;
	t_envp	*old_wd;
	t_envp	*temp;

	wd = NULL;
	old_wd = NULL;
	temp = (*mnsh)->envp;
	while (!wd || !old_wd)
	{
		if (ft_strncmp(temp->content, "PWD=", 4) == 0)
			wd = temp;
		else if (ft_strncmp(temp->content, "OLDPWD=", 7) == 0)
			old_wd = temp;
		temp = temp->next;
	}
	//test
	printf("pwd: %s\noldpwd: %s\n", wd->content, old_wd->content);
	if (wd && old_wd)
	{
		free(old_wd->content);
		old_wd->content = ft_strpathdup("OLDPWD=", (wd->content + 4));
		free(wd->content);
		wd->content = ft_strpathdup("PWD=", getcwd(NULL, 0));
	}
	//test
	printf("pwd: %s\noldpwd: %s\n", wd->content, old_wd->content);
}

int	cd(char **cmd, t_minishell **mnsh)
{
	if (cmd[1] && cmd[2])
	{
		printf("cd: too many arguments\n");
		return (1);
	}
	if (cmd[1] && chdir(cmd[1]) != 0)
	{
		printf("cd: %s: No such file or directory\n", cmd[1]);
		return (1);
	}
	else if (!cmd[1])
		chdir(getenv("HOME"));
	update_wd(mnsh);
	return (0);
}