/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 09:06:01 by dphang            #+#    #+#             */
/*   Updated: 2024/04/05 01:06:10 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_mnsh(char **envp, t_minishell **mnsh)
{
	int	i;
	int	envar;

	i = 0;
	envar = 0;
	while (envp[envar])
	{
		envar++;
	}
	*mnsh = malloc(sizeof(t_minishell));
	if (!(*mnsh))
		return ;
	(*mnsh)->envp = malloc((envar + 1) * sizeof(char *));
	if (!((*mnsh)->envp))
		return ;
	while (envp[i])
	{
		(*mnsh)->envp[i] = ft_strdup(envp[i]);
		i++;
	}
	(*mnsh)->envp[i] = '\0';
	(*mnsh)->exit_status = 0;
}

// int	main(int ac, char **av, char **envp)
// {
// 	(void)ac;
// 	t_minishell *mnsh;

// 	init_mnsh(envp, &mnsh);
// 	echo(av);
// 	//pwd();
// 	//env(av, mnsh->envp);
// 	return (0);
// }

int	main(void)
{
	char 	*input;
	t_dls *tokens;
	int		scode;
	int		exit_sig;

	exit_sig = 0;
	// setup signal
	while (!exit_sig)
	{
		// input = readline("$ ");
		input = "echo test || cat -e";
		if (!input)
			return (1);
		tokens = parse_token(input);
		// scode = exec(syntax);
	}
	// free stuffs

	return (0);
}