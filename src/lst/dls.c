/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dls.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 17:28:39 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/24 17:51:22 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_dls	*ft_dlsnew(char *content, t_type type)
{
	t_dls	*node;
	if (!content)
		return (NULL);
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
