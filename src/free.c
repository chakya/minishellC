/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:42:00 by dphang            #+#    #+#             */
/*   Updated: 2024/04/21 16:29:15 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_envp(t_envp **envp)
{
	t_envp	*temp;

	while (*envp)
	{
		temp = *envp;
        *envp = (*envp)->next;
        free(temp->content);
        free(temp);
	}
	*envp = NULL;
}

void    free_all(t_minishell **mnsh)
{
    free_envp(&((*mnsh)->envp));
	free(*mnsh);
	*mnsh = NULL;
}
