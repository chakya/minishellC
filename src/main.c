/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 09:06:01 by dphang            #+#    #+#             */
/*   Updated: 2024/04/18 21:24:51 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// void	init_mnsh(char **envp, t_minishell **mnsh)
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
// 	(*mnsh)->exit_status = 0;
// }

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

int printf_tokens(t_dls *tokens)
{
	t_dls *tmp;

	tmp = tokens;
	while (tmp)
	{
		printf("%s\n", tmp->content);
		tmp = tmp->next;
	}
	return (0);
}

// 	// free stuffs
int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	t_minishell *mnsh;

	init_mnsh(envp, &mnsh);
	char 	*input;
	t_dls *tokens;
	int		exit_sig;

	exit_sig = 0;
	while (!exit_sig)
	{
		input = readline("MiniDillon 🦊 ");
		add_history(input);
		if (!input)
			return (1);
		tokens = parse_token(input);
		mnsh->ast = parse_ast(tokens);
		execute_ast(&mnsh, NULL);
	}
	free_all(&mnsh);
	return (0);
}
