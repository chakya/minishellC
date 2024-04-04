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

/*
	Search and launch the right executable, based on the PATH variable or
	using a relative or an absolute path.

	Get the path of built-in shell commands:
		echo with -n flag								DONE, req testing
			-n : do not output the trailing newline
		cd with only a relative or absolute path		WIP
		pwd												DONE + TESTED
		export											WIP
		unset											WIP
		env												DONE + TESTED
		exit											WIP
*/

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return ((int)((unsigned char)s1[i] - (unsigned char)s2[i]));
		i++;
	}
	return (0);
}

int	is_redir(char *str)
{
	if (ft_strcmp(str, "|") == 0 || ft_strcmp(str, "<") == 0
		|| ft_strcmp(str, "<<") == 0 || ft_strcmp(str, ">") == 0
		|| ft_strcmp(str, ">>") == 0)
		return (1);
	return (0);
}

int	is_envar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (str[i + 1] >= 'A' && str[i + 1] <= 'Z'))
			return (1);
		i++;
	}
	return (0);
}

/* Have to handle single quote for environment variable
	eg.: '$HOME' should print $HOME
   $? not handled yet.
*/
void	put_envar(char *str)
{
	int		i;
	int		j;
	int		k;
	char	*name;
	char	*var_val;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (str[i + 1] >= 'A' && str[i + 1] <= 'Z'))
		{
			j = 0;
			while (str[i + j] != ' ')
			{
				j++;
			}
			name = malloc((j + 1) * sizeof(char));
			if (name == NULL)
				return ;
			k = 0;
			while (k <= j)
			{
				name[k] = str[i];
				i++;
				k++;
			}
			name[k] = '\0';
			var_val = getenv(name);
			if (var_val)
				printf("%s", var_val);
			free(name);
		}
		else
		{
			ft_putchar_fd(str[i], 1);
			i++;
		}
	}
}

int	echo(char **cmd)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if (cmd[i] && ft_strcmp(cmd[i], "-n") == 0)
	{
		flag = 1;
		i++;
	}
	while (cmd[i] && !is_redir(cmd[i]))
	{
		if (is_envar(cmd[i]))
			put_envar(cmd[i]);
		else if ((i > 1 && flag == 0) || i > 2)
			printf(" %s", cmd[i]);
		else
			printf("%s", cmd[i]);
		i++;
	}
	if (flag == 0)
		printf("\n");
	return (0);
}

int	pwd(void)
{
	char	*wd;

	wd = (char *)malloc(PATH_MAX * sizeof(char));
	if (wd != NULL)
	{
		getcwd(wd, PATH_MAX);
		printf("%s\n", wd);
		free(wd);
		return (0);
	}
	return (1);
}

int	env(char **cmd, char **envp)
{
	int	i;

	i = 0;
	if (cmd[1] && !is_redir(cmd[1]))
	{
		printf("env: '%s': No such file or directory\n", cmd[1]);
		return (1);
	}
	else
	{
		while (envp[i])
		{
			printf("%s\n", envp[i]);
			i++;
		}
	}
	return (0);
}
