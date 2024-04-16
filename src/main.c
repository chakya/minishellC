/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 09:06:01 by dphang            #+#    #+#             */
/*   Updated: 2024/04/16 16:42:25 by dphang           ###   ########.fr       */
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
	t_signals	sigs;
	char		*input;
	t_dls		*tokens;

	init_mnsh(envp, &mnsh);
	init_sigs(&sigs);
	while (!mnsh->exit_sig)
	{
		input = readline("minicharles$ ");
		add_history(input);
		if (!input)
			eof_handler(&mnsh);
		if (g_sig_received == 1)
		{
			mnsh->exit_code = 130;
			g_sig_received = 0;
		}
		tokens = parse_token(input);
		execute_ast(parse_ast(tokens), &mnsh, envp);
	}
	free_all(&mnsh);
	return (mnsh->exit_code);
}
