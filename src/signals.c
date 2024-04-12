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
}

int    eof_handler(t_minishell **mnsh)
{
    printf("exit\n");
    mnsh_exit(mnsh);
    return (1);
}

void    init_sigs(void)
{
    g_sigs.sigint_sa.sa_flags = 0;
	sigemptyset(&(g_sigs.sigint_sa.sa_mask));
	g_sigs.sigint_sa.sa_handler = &sigint_handler;
    g_sigs.sigquit_sa.sa_flags = 0;
	sigemptyset(&(g_sigs.sigquit_sa.sa_mask));
	g_sigs.sigquit_sa.sa_handler = SIG_IGN;
}