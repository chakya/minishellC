/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 14:57:08 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/24 14:57:30 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_dls	*check_token(t_dls *tokens, t_dls *token)

{
	if (tokens && (token->type >= T_PIPE))
	{
		ft_putstr_fd("Syntax Error\n", 2);
		free_tokens(tokens);
		tokens = NULL;
	}

	if (tokens && (token->type >= T_COL && token->type <= T_AND))
	{
		ft_putstr_fd("Not supported\n", 2);
		free_tokens(tokens);
		tokens = NULL;
	}
	return (tokens);
}

int	token_valid(t_dls *token)
{
	return ((token->type == T_PIPE && token->prev
			&& token->prev->type >= T_PIPE) || (token->type >= T_INPUT
			&& token->prev && token->prev->type >= T_INPUT)
		|| (token->type == T_PIPE && token->prev && token->prev->type >= T_PIPE)
		|| (token->type >= T_COL && token->type <= T_AND));
}

t_dls	*parse_token(char *input, t_minishell **mnsh)
{
	t_dls	*tokens;
	t_dls	*token;

	tokens = NULL;
	token = NULL;
	ft_skipspaces(&input);
	while (*input)
	{
		if (get_ops_type(input))
			token = tokenize_operation(&input);
		else
			token = tokenize_param(&input, mnsh, token);
		ft_dlsadd_back(&tokens, token);
		if (!tokens)
			return (free_tokens(tokens));
		if (token_valid(token))
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
