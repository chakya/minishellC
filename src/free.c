/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:42:00 by dphang            #+#    #+#             */
/*   Updated: 2024/04/09 12:42:01 by dphang           ###   ########.fr       */
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
}

void    free_all(t_minishell **mnsh)
{
    free_envp(&((*mnsh)->envp));
    free(*mnsh);
}