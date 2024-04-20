/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/08 16:37:48 by dphang            #+#    #+#             */
/*   Updated: 2024/04/16 16:44:01 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_envp	*newenvp(char *var)
{
	t_envp	*node;

	node = malloc(sizeof(t_envp));
	node->content = ft_strdup(var);
	node->next = NULL;
	return (node);
}

t_envp	*init_envp(char **envp)
{
	t_envp	*var;
	t_envp	*temp;
	int		i;

	i = 0;
	var = newenvp(envp[i++]);
	temp = var;
	while (envp[i])
	{
		temp->next = newenvp(envp[i]);
		if (temp->next)
			temp = temp->next;
		i++;
	}
	return (var);
}

void	init_mnsh(char **envp, t_minishell **mnsh)
{
	*mnsh = malloc(sizeof(t_minishell));
	(*mnsh)->envp = init_envp(envp);
	(*mnsh)->exit_sig = 0;
	(*mnsh)->sgl_quote = 0;
	(*mnsh)->dbl_quote = 0;
	(*mnsh)->exit_code = 0;
	(*mnsh)->ast = NULL;
	(*mnsh)->opipe = NULL;
}
