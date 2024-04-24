/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:59:06 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/24 15:52:27 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handler(int signum)
{
	if (signum == SIGINT)
	{
		g_sig_received = 1;
	}
}

void	exe_pipe_child(t_ast **children, int *fd, int *opipe,
		t_minishell **mnsh)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (children[1])
		dup2(fd[1], STDOUT_FILENO);
	if (opipe)
		dup2(opipe[0], STDIN_FILENO);
	if (fd[0])
	{
		close(fd[0]);
		close(fd[1]);
	}
	if (opipe)
	{
		close(opipe[0]);
		close(opipe[1]);
	}
	execute_tokens((*children)->tokens, mnsh);
	exit(1);
}

int	execute_pipe(t_ast **children, int *opipe, t_minishell **mnsh)
{
	int	fd[2];
	int	id;

	fd[0] = 0;
	fd[1] = 0;
	if (!*children)
		return (1);
	if (children[1])
		pipe(fd);
	id = fork();
	if (id == 0)
		exe_pipe_child(children, fd, opipe, mnsh);
	else
	{
		if (opipe)
		{
			close(opipe[0]);
			close(opipe[1]);
		}
		signal(SIGINT, handler);
		execute_pipe(&children[1], fd, mnsh);
		waitpid(id, NULL, 0);
		signal(SIGINT, &sigint_handler);
	}
	return (0);
}

void	non_builtin(t_minishell **mnsh)
{
	int	id;
	int	exit_status;

	id = fork();
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (execute_tokens((*mnsh)->ast->tokens, mnsh))
			exit(1);
		exit(0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(id, &exit_status, 0);
		exit_child(mnsh, exit_status);
		signal(SIGINT, sigint_handler);
	}
}

int	execute_ast(t_minishell **mnsh, int *opipe)
{
	if (!(*mnsh)->ast)
		return (0);
	if ((*mnsh)->ast->type == T_PIPE)
	{
		execute_pipe((*mnsh)->ast->children, opipe, mnsh);
		if (g_sig_received)
			printf("\n");
	}
	else if ((*mnsh)->ast->type == T_CMD)
	{
		if (is_builtins(&(*mnsh)->ast->tokens->content))
		{
			execute_tokens((*mnsh)->ast->tokens, mnsh);
			dup2((*mnsh)->io[0], STDIN_FILENO);
			dup2((*mnsh)->io[1], STDOUT_FILENO);
		}
		else
			non_builtin(mnsh);
	}
	free_ast((*mnsh)->ast);
	(*mnsh)->ast = NULL;
	return (1);
}
