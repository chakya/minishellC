/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:59:06 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/22 22:27:08 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_dls	*check_token(t_dls *tokens, t_dls *token)

{
	if (tokens && (token->type == T_PIPE || token->type >= T_INPUT))
	{
		ft_putstr_fd("Syntax Error\n", 2);
		free_tokens(tokens);
		tokens = NULL;
	}
	else if (tokens && (token->type >= T_COL && token->type <= T_AND))
	{
		ft_putstr_fd("Not supported\n", 2);
		free_tokens(tokens);
		tokens = NULL;
	}
	return (tokens);
}

t_dls	*parse_token(char *input)
{
	t_dls	*tokens;
	t_dls	*token;

	tokens = NULL;
	ft_skipspaces(&input);
	while (*input)
	{
		if (get_ops_type(input))
			token = tokenize_operation(&input);
		else
			token = tokenize_param(&input);
		ft_dlsadd_back(&tokens, token);
		if (!tokens)
		{
			free_tokens(tokens);
			return (NULL);
		}
		if (token->type == T_PIPE && token->prev
			&& token->prev->type >= T_INPUT)
			break ;
		ft_skipspaces(&input);
	}
	tokens = check_token(tokens, token);
	return (tokens);
}

int	count_args(t_dls *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == T_ARG)
			count++;
		tokens = tokens->next;
	}
	return (count);
}

char	**parse_to_arg(t_dls *tokens)
{
	t_dls	*tmp;
	char	**av;
	int		i;

	i = count_args(tokens);
	if (i == 0)
		return (NULL);
	av = (char **)malloc((i + 1) * sizeof(char *));
	if (!av)
		return (0);
	i = 0;
	tmp = tokens;
	while (tmp)
	{
		if (tmp->type == T_ARG)
		{
			av[i] = ft_strdup(tmp->content);
			i++;
		}
		tmp = tmp->next;
	}
	av[i] = NULL;
	return (av);
}

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
				{
					(*mnsh)->exit_code = WEXITSTATUS(exit_status);
					if (g_sig_received)
						printf("\n");
				}
				signal(SIGINT, sigint_handler);
			}
		}
	}
	free_ast((*mnsh)->ast);
	(*mnsh)->ast = NULL;
	return (1);
}
