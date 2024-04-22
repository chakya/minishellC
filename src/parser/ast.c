/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 21:43:14 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/22 22:13:21 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**process_av(t_dls *tokens, t_minishell **mnsh)
{
	t_dls	*tmp;
	char	**av;
	char	*dollar_var;

	tmp = tokens;
	while (tmp)
	{
		if (ft_strchr(tmp->content, '\'') || ft_strchr(tmp->content, '\"')
			|| envar_exist(tmp->content))
		{
			dollar_var = parse_string(tmp->content, mnsh);
			if (dollar_var[0] == '\0' && tmp->content[0])
				tmp->type = T_EMPTY;
			free(tmp->content);
			tmp->content = dollar_var;
		}
		tmp = tmp->next;
	}
	av = parse_to_arg(tokens);
	return (av);
}

int	delim_check(char *hline, char *delim)
{
	while (*hline)
	{
		if (*delim == '\'' || *delim == '"')
			delim++;
		if (*delim == *hline)
		{
			hline++;
			delim++;
		}
		else
		{
			return (0);
		}
	}
	while (*delim)
	{
		if (*delim != '\'' && *delim != '"')
			return (0);
	}
	return (1);
}

t_type	check_redirect(t_dls *tokens)
{
	while (tokens)
	{
		if (tokens->type >= T_INPUT && tokens->type <= T_APPEND)
			return (tokens->type);
		tokens = tokens->next;
	}
	return (T_CMD);
}

void	init_ast(t_ast *ast)
{
	ast->type = T_INVALID;
	ast->tokens = NULL;
	ast->children = NULL;
}

t_ast	*parse_ast(t_dls *tokens)
{
	t_ast	*ast;
	int		count;

	ast = (t_ast *)malloc(sizeof(t_ast));
	init_ast(ast);
	if (!ast)
		return (NULL);
	count = count_pipe(tokens);
	if (count > 0)
	{
		ast->type = T_PIPE;
		ast->children = populate_children(tokens, count);
	}
	else
	{
		ast->tokens = tokens;
		check_heredoc(tokens);
		ast->type = T_CMD;
	}
	return (ast);
}
