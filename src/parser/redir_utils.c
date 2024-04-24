/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 16:27:29 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/24 16:28:49 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	interupt_handler(int signum)
{
	if (signum == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
		ft_putstr_fd("\n", 1);
		g_sig_received = 1;
		exit(130);
	}
}

void	fork_heredoc(int pid, int fd[2], char *delim, t_minishell **mnsh)
{
	char	*hline;
	char	*parsed;

	if (!pid)
	{
		signal(SIGINT, interupt_handler);
		while (delim)
		{
			hline = readline("> ");
			if (!hline)
				break ;
			if (delim_check(hline, delim))
				break ;
			parsed = parse_string(hline, mnsh);
			ft_putstr_fd((parsed), fd[1]);
			ft_putstr_fd("\n", fd[1]);
			free(parsed);
		}
		close(fd[1]);
		close(fd[0]);
		exit(0);
	}
}
