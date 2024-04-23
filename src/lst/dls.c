/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dls.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:28:39 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/23 11:18:02 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_dls	*ft_dlsnew(char *content, t_type type)
{
	t_dls	*node;

	node = malloc(sizeof(t_dls));
	if (!node)
		return (NULL);
	node->content = content;
	node->type = type;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	ft_dlsadd_back(t_dls **lst, t_dls *new)
{
	t_dls	*tmp;

	if (!lst || !new)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		tmp = *lst;
		while (tmp->next)
		{
			tmp = tmp->next;
		}
		new->prev = tmp;
		tmp->next = new;
	}
}
