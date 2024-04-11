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

int	is_envar(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			return (1);
		// else if (str[i] == '$' && str[i + 1] == '?')
		// 	return (2);
		i++;
	}
	return (0);
}


// More than 25 lines
void	put_enval(char *str, int *i)
{
	int		j;
	int		k;
	char	*name;
	char	*var_val;

	j = 0;
	while (str[*i] == '$' && (ft_isalnum(str[*i + j + 1]) || str[*i + j + 1] == '_'))
	{
		j++;
	}
	name = malloc((j + 1) * sizeof(char));
	if (name == NULL)
		return ;
	k = 0;
	while (k <= j)
	{
		name[k] = str[*i];
		i++;
		k++;
	}
	name[k] = '\0';
	var_val = getenv(name);
	if (var_val)
		printf("%s", var_val);
	free(name);
}

/* Have to handle single quote for environment variable
	eg.: '$HOME' should print $HOME
   $? not handled yet.
*/
void	put_envar(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && ft_isalnum(str[i + 1]))
		{
			put_enval(str, &i);
		}
		// else if (str[i] == '$' && str[i + 1] == '?')
		// {
		// 	// handle exit status
		// }
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