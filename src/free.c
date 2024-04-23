/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 12:42:00 by dphang            #+#    #+#             */
/*   Updated: 2024/04/23 12:29:36 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_envp(t_envp **envp)
{
	t_envp	*temp;

	while (*envp)
	{
		temp = *envp;
		*envp = (*envp)->next;
		free(temp->content);
		free(temp);
	}
	*envp = NULL;
}

void	free_all(t_minishell **mnsh)
{
	free_envp(&((*mnsh)->envp));
	free(*mnsh);
	*mnsh = NULL;
}

void	free_tokens(t_dls *tokens)
{
	t_dls	*temp;

	while (tokens)
	{
		temp = tokens;
		tokens = tokens->next;
		free(temp->content);
		free(temp);
	}
}

void	free_ast(t_ast *ast)
{
	int	i;

	if (ast->type == T_PIPE)
	{
		i = 0;
		while (ast->children[i])
		{
			free_ast(ast->children[i]);
			ast->children[i] = NULL;
			i++;
		}
		free(ast->children);
		ast->children = NULL;
	}
	free_tokens(ast->tokens);
	ast->tokens = NULL;
	free(ast);
	ast = NULL;
}

void	free_arr(char ***arr)
{
	int	i;

	i = 0;
	while ((*arr)[i])
	{
		free((*arr)[i]);
		i++;
	}
	free(*arr);
}
