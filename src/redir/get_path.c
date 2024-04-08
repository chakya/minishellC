/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_path.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 17:25:39 by dphang            #+#    #+#             */
/*   Updated: 2024/04/08 17:25:40 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**get_path(t_minishell **mnsh)
{
	char	**path;
	t_envp	*temp;

	temp = (*mnsh)->envp;
	while (ft_strncmp(temp->content, "PATH=", 5) != 0)
	{
		temp = temp->next;
	}
    path = ft_split((temp->content + 5), ':');
    int i = 0;
    while (path[i])
    {
        printf("%s\n", path[i]);
        i++;
    }
    return (path);
}