/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 21:00:17 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/23 19:40:26 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	count_pipe(t_dls *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == T_PIPE)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

t_dls	*copy_prev(t_dls **tokens)
{
	t_dls	*temp;

	temp = (*tokens)->prev;
	if ((*tokens)->next)
	{
		*tokens = (*tokens)->next;
		free((*tokens)->prev->content);
		free((*tokens)->prev);
		(*tokens)->prev = NULL;
		temp->next = NULL;
	}
	else
	{
		temp = *tokens;
		*tokens = NULL;
	}
	while (temp->prev)
		temp = temp->prev;
	return (temp);
}

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

void	check_heredoc(t_dls *tokens, t_minishell **mnsh)
{
	char	*delim;
	char	*hline;
	char	*parsed;
	int		fd[2];
	int		pid;

	while (tokens)
	{
		if (tokens->type == T_HEREDOC)
		{
			if (!tokens->next)
			{
				ft_putstr_fd("Syntax Error", 2);
				exit(2);
			}
			delim = tokens->next->content;
			pipe(fd);
			pid = fork();
			if (!pid)
			{
				signal(SIGINT, interupt_handler);
				while (delim)
				{
					hline = readline("> ");
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
			close(fd[1]);
			tokens->heredoc = fd[0];
			signal(SIGINT, SIG_IGN);
			waitpid(pid, NULL, 0);
			signal(SIGINT, sigint_handler);
		}
		tokens = tokens->next;
	}
}

t_ast	**populate_children(t_dls *tokens, int count, t_minishell **mnsh)
{
	t_ast	**children;
	int		i;

	i = 0;
	children = (t_ast **)malloc(sizeof(t_ast *) * (count + 2));
	while (tokens)
	{
		if (tokens->type == T_PIPE || tokens->next == NULL)
		{
			children[i] = (t_ast *)malloc(sizeof(t_ast));
			children[i]->tokens = copy_prev(&tokens);
			check_heredoc(children[i]->tokens, mnsh);
			children[i]->type = T_CMD;
			i++;
		}
		else
			tokens = tokens->next;
	}
	children[i] = NULL;
	return (children);
}
