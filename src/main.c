/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 09:06:01 by dphang            #+#    #+#             */
/*   Updated: 2024/04/21 19:27:50 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	g_sig_received = 0;

int	printf_tokens(t_dls *tokens)
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

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	(void)av;
	t_minishell	*mnsh;
	//t_signals	sigs;
	char		*input;
	t_dls		*tokens;
	int 		exit_code;

	init_mnsh(envp, &mnsh);
	signal(SIGINT, &sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	//init_sigs(&sigs);
	while (!mnsh->exit_sig)
	{
		input = readline("MiniDillon 🦊 ");
		if (!input)
			eof_handler(&mnsh);
		if (input[0] == '\0')
			continue ;
		add_history(input);
		if (g_sig_received == 1)
		{
			mnsh->exit_code = 130;
			g_sig_received = 0;
		}
		tokens = parse_token(input);
		free(input);
		if (!tokens)
			continue ;
		mnsh->ast = parse_ast(tokens);
		execute_ast(&mnsh, NULL);
	}
	exit_code = mnsh->exit_code;
	free_all(&mnsh);
	clear_history();
	return (exit_code);
}
