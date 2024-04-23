/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 21:59:03 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/23 11:16:01 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	envar_exist(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			return (1);
		else if (str[i] == '$' && str[i + 1] == '?')
			return (2);
		i++;
	}
	return (0);
}

int	is_envar(char *str)
{
	if (str[0] == '$' && (ft_isalnum(str[1]) || str[1] == '_' || str[1] == '?'))
		return (1);
	return (0);
}

char	*enval(char *var, t_envp *envp)
{
	while (envp && ft_strncmp(var, envp->content, ft_strlen(var)) != 0)
	{
		envp = envp->next;
	}
	if (envp)
		return (envp->content + ft_strlen(var));
	else
		return ("");
}
