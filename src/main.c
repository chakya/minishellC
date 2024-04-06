/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 09:06:01 by dphang            #+#    #+#             */
/*   Updated: 2024/04/04 15:02:13 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// void	init_envp(char **envp, t_minishell **mnsh)
// {
// 	int	i;
// 	int	envar;

// 	i = 0;
// 	envar = 0;
// 	while (envp[envar])
// 	{
// 		envar++;
// 	}
// 	*mnsh = malloc(sizeof(t_minishell));
// 	if (!(*mnsh))
// 		return ;
// 	(*mnsh)->envp = malloc((envar + 1) * sizeof(char *));
// 	if (!((*mnsh)->envp))
// 		return ;
// 	while (envp[i])
// 	{
// 		(*mnsh)->envp[i] = ft_strdup(envp[i]);
// 		i++;
// 	}
// 	(*mnsh)->envp[i] = '\0';
// }

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
	int	i;

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
	(*mnsh)->exit_status = 0;
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	t_minishell *mnsh;

	init_mnsh(envp, &mnsh);
	//echo(av);
	//pwd();
	//env(av, mnsh);
	//cd(av, &mnsh);
	env(av, mnsh);
	return (0);
}
