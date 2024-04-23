/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_token.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 21:41:11 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/23 18:57:03 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	empty_string(t_dls *tokens)
{
	char	*str;

	if (!tokens)
		return ;
	str = tokens->content;
	while (*str)
	{
		*str = '\0';
		str++;
	}
	tokens->type = T_EMPTY;
}

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

int	proc_redir(t_dls *tokens, t_minishell *mnsh)
{
	int	fd[2] = {-1, -1};

	while (tokens)
	{
		if (tokens->type == T_INPUT)
		{
			fd[0] = open(tokens->next->content, O_RDONLY);
			if (fd[0] < 0)
			{
				perror("");
				return (1);
			}
			rem2(tokens);
		}
		else if (tokens->type == T_HEREDOC)
		{
			fd[0] = tokens->heredoc;
			rem2(tokens);
		}
		else if (tokens->type == T_TRUNC)
		{
			fd[1] = open(tokens->next->content, O_WRONLY | O_CREAT | O_TRUNC,
					0644);
			if (fd[1] < 0)
			{
				perror("");
				return (1);
			}
			rem2(tokens);
		}
		else if (tokens->type == T_APPEND)
		{
			fd[1] = open(tokens->next->content, O_WRONLY | O_CREAT | O_APPEND,
					0644);
			if (fd[1] < 0)
			{
				perror("");
				return (1);
			}
			rem2(tokens);
		}
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
	av = process_av(tokens, mnsh);
	if (!av)
		return (0);
	(*mnsh)->exit_code = excu(av, mnsh);
	return (0);
}
