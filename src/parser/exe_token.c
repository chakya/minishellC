/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 21:41:11 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/24 17:03:25 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	rem2(t_dls *tokens)
{
	empty_string(tokens);
	if (tokens->next)
		empty_string(tokens->next);
	else
	{
		printf("Invalid syntax");
	}
}

int	redir_error(int f)
{
	if (f < 0)
	{
		perror("");
		return (1);
	}
	return (0);
}

int	check_redir(int *fd, t_dls *tokens)
{
	if (tokens->type == T_INPUT)
	{
		fd[0] = open(tokens->next->content, O_RDONLY);
		if (redir_error(fd[0]))
			return (1);
	}
	else if (tokens->type == T_HEREDOC)
		fd[0] = tokens->heredoc;
	else if (tokens->type == T_TRUNC || tokens->type == T_APPEND)
	{
		if (tokens->type == T_TRUNC)
			fd[1] = open(tokens->next->content, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
		else if (tokens->type == T_APPEND)
			fd[1] = open(tokens->next->content, O_WRONLY | O_CREAT | O_APPEND,
					0644);
		if (redir_error(fd[1]))
			return (1);
	}
	if (tokens->type >= T_INPUT)
		rem2(tokens);
	return (0);
}

int	proc_redir(t_dls *tokens, t_minishell *mnsh)
{
	int	fd[2];

	fd[0] = -1;
	fd[1] = -1;
	while (tokens)
	{
		if (check_redir(fd, tokens))
			return (1);
		tokens = tokens->next;
	}
	mnsh->io[0] = dup(STDIN_FILENO);
	mnsh->io[1] = dup(STDOUT_FILENO);
	if (fd[0] > 2)
	{
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
	}
	else if (fd[1] > 2)
	{
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
	}
	return (0);
}

int	execute_tokens(t_dls *tokens, t_minishell **mnsh)
{
	char	**av;

	if (proc_redir(tokens, *mnsh))
		return (1);
	av = process_av(tokens);
	if (!av)
		return (0);
	(*mnsh)->exit_code = excu(av, mnsh);
	return (0);
}
