/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:59:06 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/24 14:57:14 by cwijaya          ###   ########.fr       */
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

int	execute_pipe(t_ast **children, int *opipe, t_minishell **mnsh)
{
	int	fd[2] = {0, 0};
	int	id;

	if (!*children)
		return (1);
	if (children[1])
		pipe(fd);
	id = fork();
	if (id == 0)
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

int	execute_ast(t_minishell **mnsh, int *opipe)
{
	int	id;
	int	exit_status;

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
		{
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
				if (WIFEXITED(exit_status))
					(*mnsh)->exit_code = WEXITSTATUS(exit_status);
				if (WIFSIGNALED(exit_status))
				{
					if (WTERMSIG(exit_status) == SIGINT)
					{
						g_sig_received = 1;
						printf("\n");
					}
				}
				signal(SIGINT, sigint_handler);
			}
		}
	}
	free_ast((*mnsh)->ast);
	(*mnsh)->ast = NULL;
	return (1);
}
