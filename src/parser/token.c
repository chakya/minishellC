/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 20:40:17 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/24 17:54:57 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_type	get_ops_type(char *ops)
{
	if (!ft_strncmp(ops, "||", 2))
		return (T_OR);
	else if (!ft_strncmp(ops, "&&", 2))
		return (T_AND);
	else if (!ft_strncmp(ops, ">>", 2))
		return (T_APPEND);
	else if (!ft_strncmp(ops, "<<", 2))
		return (T_HEREDOC);
	else if (*ops == '(')
		return (T_OB);
	else if (*ops == ')')
		return (T_CB);
	else if (*ops == '|')
		return (T_PIPE);
	else if (*ops == '<')
		return (T_INPUT);
	else if (*ops == '>')
		return (T_TRUNC);
	else if (*ops == ';')
		return (T_COL);
	else
		return (T_INVALID);
}

char	*ft_strndup(const char *s, int len)
{
	int		i;
	char	*dup;

	i = 0;
	dup = (char *)malloc((len + 1) * sizeof(char));
	if (!dup)
		return (((void *)0));
	while (s[i] && i < len)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

t_dls	*tokenize_operation(char **input)
{
	t_dls	*token;

	if (!ft_strncmp(*input, "&&", 2) || !ft_strncmp(*input, "||", 2)
		|| !ft_strncmp(*input, ">>", 2) || !ft_strncmp(*input, "<<", 2))
	{
		token = ft_dlsnew(ft_strndup(*input, 2), get_ops_type(*input));
		(*input)++;
	}
	else
		token = ft_dlsnew(ft_strndup(*input, 1), get_ops_type(*input));
	(*input)++;
	return (token);
}

t_dls	*tokenize_param(char **input, t_minishell **mnsh, t_dls *prev)
{
	t_dls	*token;
	int		l;
	char	*str;

	l = 0;
	while ((*input)[l] && !is_delim((*input) + l))
	{
		if ((*input)[l] == '"' || (*input)[l] == '\'')
			l += loop_quote((*input) + l);
		else
			l++;
	}
	if (l == 0)
		return (NULL);
	if (!prev || (prev && prev->type != T_HEREDOC))
		str = parse_string(ft_strndup(*input, l), mnsh);
	else
		str = ft_strndup(*input, l);
	token = ft_dlsnew(str, T_ARG);
	*input += l;
	return (token);
}

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
