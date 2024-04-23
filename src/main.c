/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 09:06:01 by dphang            #+#    #+#             */
/*   Updated: 2024/04/23 19:34:12 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_sig_received = 0;

void	minishell(t_minishell **mnsh)
{
	char	*input;
	t_dls	*tokens;

	while (!(*mnsh)->exit_sig)
	{
		input = readline("MiniCharles&Dillon 🦊$ ");
		if (!input)
			eof_handler(mnsh);
		if (input[0] == '\0')
			continue ;
		add_history(input);
		if (g_sig_received == 1)
		{
			(*mnsh)->exit_code = 130;
			g_sig_received = 0;
		}
		tokens = parse_token(input, mnsh);
		free(input);
		if (!tokens)
			continue ;
		(*mnsh)->ast = parse_ast(tokens, mnsh);
		execute_ast(mnsh, NULL);
	}
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	*mnsh;
	int			exit_code;

	(void)ac;
	(void)av;
	init_mnsh(envp, &mnsh);
	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	minishell(&mnsh);
	exit_code = mnsh->exit_code;
	free_all(&mnsh);
	clear_history();
	return (exit_code);
}

// int	main(int ac, char **av, char **envp)
// {
// 	t_minishell	*mnsh;
// 	char		*input;
// 	t_dls		*tokens;
// 	int			exit_code;

// 	(void)ac;
// 	(void)av;
// 	init_mnsh(envp, &mnsh);
// 	signal(SIGINT, &sigint_handler);
// 	signal(SIGQUIT, SIG_IGN);
// 	while (!mnsh->exit_sig)
// 	{
// 		input = readline("MiniDillon 🦊 ");
// 		if (!input)
// 			eof_handler(&mnsh);
// 		if (input[0] == '\0')
// 			continue ;
// 		add_history(input);
// 		if (g_sig_received == 1)
// 		{
// 			mnsh->exit_code = 130;
// 			g_sig_received = 0;
// 		}
// 		tokens = parse_token(input);
// 		free(input);
// 		if (!tokens)
// 			continue ;
// 		mnsh->ast = parse_ast(tokens);
// 		execute_ast(&mnsh, NULL);
// 	}
// 	exit_code = mnsh->exit_code;
// 	free_all(&mnsh);
// 	clear_history();
// 	return (exit_code);
// }
