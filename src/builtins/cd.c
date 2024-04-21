/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <dphang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:02:40 by dphang            #+#    #+#             */
/*   Updated: 2024/04/16 16:59:14 by dphang           ###   ########.fr       */
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
	dup = malloc((len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	while (path[i])
	{
		dup[i] = path[i];
		i++;
	}
	while (value[j])
	{
		dup[i + j] = value[j];
		j++;
	}
	dup[i + j] = '\0';
	return (dup);
}

void	update_wd(t_minishell **mnsh)
{
	t_envp	*wd;
	t_envp	*old_wd;
	t_envp	*temp;
	char	*new_wd;

	wd = NULL;
	old_wd = NULL;
	temp = (*mnsh)->envp;
	while (temp && (!wd || !old_wd))
	{
		if (ft_strncmp(temp->content, "PWD=", 4) == 0)
			wd = temp;
		else if (ft_strncmp(temp->content, "OLDPWD=", 7) == 0)
			old_wd = temp;
		temp = temp->next;
	}
	if (wd && old_wd)
	{
		free(old_wd->content);
		old_wd->content = ft_strpathdup("OLDPWD=", (wd->content + 5));
		free(wd->content);
		new_wd = (char *)malloc(PATH_MAX * sizeof(char));
		getcwd(new_wd, PATH_MAX);
		wd->content = ft_strpathdup("PWD=", new_wd);
		free(new_wd);
	}
}

int	cd(char **cmd, t_minishell **mnsh)
{
	if (cmd[1] && cmd[2])
	{
		printf("cd:");
		ft_putstr_fd(" too many arguments\n", 2);
		return (1);
	}
	if (cmd[1] && chdir(cmd[1]) != 0)
	{
		printf("cd: %s:", cmd[1]);
		ft_putstr_fd(" No such file or directory\n", 2);
		return (1);
	}
	else if (!cmd[1])
		chdir(getenv("HOME"));
	update_wd(mnsh);
	return (0);
}
