/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 20:32:56 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/23 21:27:45 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_delim(char *c)
{
	return (*c == ' ' || *c == '\t' || *c == '<' || *c == '>' || *c == '|'
		|| *c == '(' || *c == ')' || *c == '\0' || !ft_strncmp(c, "&&", 2));
}

int	ft_isspace(char c)
{
	return (c == ' ' || c == '\t');
}

int	loop_quote(char *c)
{
	int		i;
	int		is_closed;
	char	quote;

	quote = *c;
	c++;
	i = 1;
	is_closed = 0;
	while (*c)
	{
		i++;
		if (*c == quote && !is_closed)
			is_closed = 1;
		else if ((*c == '\'' || *c == '"') && is_closed)
		{
			is_closed = 0;
			quote = *c;
		}
		c++;
		if ((ft_isspace(*c) || is_delim(c)) && is_closed)
			return (i);
	}
	return (i);
}

int	ft_isoperation(char *str)
{
	return (*str == '(' || *str == ')' || *str == '|' || !ft_strncmp(str, "&&",
			2) || !ft_strncmp(str, "||", 2) || *str == '<' || *str == '>'
		|| *str == ';');
}

void	ft_skipspaces(char **str)
{
	while (ft_isspace(**str))
		(*str)++;
}
