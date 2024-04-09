/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <dphang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:02:40 by dphang            #+#    #+#             */
/*   Updated: 2024/04/05 09:50:07 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 || !s2)
	{
		if (s1 == s2)
			return (0);
		else if (!s1)
			return (-1);
		else
			return (1);
	}
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
