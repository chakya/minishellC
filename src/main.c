/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 09:06:01 by dphang            #+#    #+#             */
/*   Updated: 2024/04/10 20:25:05 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_signals	g_sigs;

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
	init_sigs();
	sigaction(SIGINT, &g_sigs.sigint_sa, NULL);
	sigaction(SIGQUIT, &g_sigs.sigquit_sa, NULL);
	// int		scode;
	//int		exit_sig;

	//exit_sig = 0;
	// setup signal
	while (!mnsh->exit_sig)
	{
		input = readline("minicharles$ ");
		// input = "echo '  test  i'";
		add_history(input);
		if (!input)
			return (eof_handler(&mnsh));
		tokens = parse_token(input);
		// printf_tokens(tokens);
		execute_ast(parse_ast(tokens), &mnsh, envp);
		// scode = exec(syntax);
	}
	free_all(&mnsh);
	return (0);
}