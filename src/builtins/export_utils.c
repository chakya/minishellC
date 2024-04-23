/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 11:27:33 by dphang            #+#    #+#             */
/*   Updated: 2024/04/23 11:32:58 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_envp	*envp_exist(char *envp, t_minishell **mnsh)
{
	int		var;
	t_envp	*temp;

	var = 0;
	while (envp[var])
	{
		var++;
		if (envp[var] == '=')
			break ;
		else if (envp[var] != '=' && envp[var + 1] == '\0')
			return (NULL);
	}
	temp = (*mnsh)->envp;
	while (temp)
	{
		if (ft_strncmp(temp->content, envp, var) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

void	add_exp(char *envp, t_minishell **mnsh)
{
	t_envp	*temp;

	if (ft_strchr(envp, '='))
	{
		if (!(*mnsh)->envp)
			(*mnsh)->envp = newenvp(envp);
		else
		{
			temp = (*mnsh)->envp;
			while (temp->next)
			{
				temp = temp->next;
			}
			temp->next = newenvp(envp);
		}
	}
}

char	*enclose_val(char *envp)
{
	int		i;
	int		j;
	char	*res;
	int		equal;

	i = 0;
	j = 0;
	equal = 0;
	res = malloc((ft_strlen(envp) + 3) * sizeof(char));
	while (envp[i])
	{
		if (envp[i] == '=' && equal == 0)
		{
			res[j++] = envp[i++];
			res[j++] = '"';
			equal = 1;
		}
		else
			res[j++] = envp[i++];
	}
	res[j] = '"';
	res[j + 1] = '\0';
	return (res);
}

t_envp	*dup_envp(t_envp *envp)
{
	t_envp	*dup;
	t_envp	*dup_temp;
	t_envp	*temp;
	char	*str;

	str = enclose_val(envp->content);
	dup = newenvp(str);
	free(str);
	dup_temp = dup;
	temp = envp->next;
	while (temp)
	{
		if (ft_strncmp(temp->content, "_=", 2) != 0)
		{
			str = enclose_val(temp->content);
			dup_temp->next = newenvp(str);
			free(str);
			dup_temp = dup_temp->next;
		}
		temp = temp->next;
	}
	return (dup);
}

int	is_validenvar(char *envp)
{
	int	i;

	i = 0;
	if (envp[i] == '=' || ft_isdigit(envp[i]))
		return (0);
	while (envp[i] && envp[i] != '=')
	{
		if (!(ft_isalnum(envp[i]) || envp[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}
