/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 10:50:02 by dphang            #+#    #+#             */
/*   Updated: 2024/04/12 10:50:04 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void    sigint_handler(int sig)
{
    (void)sig;
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

int    eof_handler(t_minishell **mnsh)
{
    printf("exit\n");
    mnsh_exit(mnsh);
    return (1);
}

void    init_sigs(t_signals *sigs)
{
    (*sigs).sigint_sa.sa_flags = 0;
	sigemptyset(&((*sigs).sigint_sa.sa_mask));
	(*sigs).sigint_sa.sa_handler = &sigint_handler;
    (*sigs).sigquit_sa.sa_flags = 0;
	sigemptyset(&((*sigs).sigquit_sa.sa_mask));
	(*sigs).sigquit_sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &(*sigs).sigint_sa, NULL);
	sigaction(SIGQUIT, &(*sigs).sigquit_sa, NULL);
}