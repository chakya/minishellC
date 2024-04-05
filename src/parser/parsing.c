/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/04 15:59:06 by cwijaya           #+#    #+#             */
/*   Updated: 2024/04/05 01:23:33 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_delim(char *c)
{
	return (*c == ' ' || *c == '\t' || *c == '<' || *c == '>' || *c == '|' || *c == '(' || *c == ')' || *c == '\0' || !ft_strncmp(c, "&&", 2));
}

int loop_quote(char **c)
{
	int i;
	char quote;

	quote = **c;
	(*c)++;
	i = 1;
	while (*c)
	{
		if (**c == quote)
			return (i);
		i++;
		(*c)++;
	}
	return (0);
}

int ft_isspace(char c)
{
    return (c == ' ' || c == '\t');
}

int ft_isoperation(char *str)
{
    return (*str == '(' || *str == ')' || *str == '|' || !ft_strncmp(str, "&&", 2) || !ft_strncmp(str, "||", 2)
			|| *str == '<' || *str == '>' || *str == ';');
}

void ft_skipspaces(char **str)
{
    while (ft_isspace(**str))
       ( *str)++;
}

t_type get_ops_type(char *ops)
{
	if (!ft_strncmp(ops, "&&", 2))
		return (T_AND);
	else if (!ft_strncmp(ops, "||", 2))
		return (T_OR);
	else if (!ft_strncmp(ops, ">>", 2))
		return (T_DR);
	else if (!ft_strncmp(ops, "<<", 2))
		return (T_DL);
	else if (*ops == '(')
		return (T_OPEN_B);
	else if (*ops == ')')
		return (T_CLOSE_B);
	else if (*ops == '|')
		return (T_PIPE);
	else if (*ops == '<')
		return (T_L);
	else if (*ops == '>')
		return (T_R);
	else if (*ops == ';')
		return (T_COL);
	else
		return (T_INVALID);
}

char	*ft_strndup(const char *s, int n)
{
	int		i;
	int		len;
	char	*dup;

	i = 0;
	len = 0;
	while (s[len])
		len++;
	dup = (char *)malloc((len + 1) * sizeof(char));
	if (!dup)
		return (((void *) 0));
	while (s[i] && i < n)
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

t_dls *tokenize_operation(char **input)
{
	t_dls *token;
	
	if (!ft_strncmp(*input, "&&", 2) || !ft_strncmp(*input, "||", 2) || !ft_strncmp(*input, ">>", 2) || !ft_strncmp(*input, "<<", 2))
	{
		token = ft_dlsnew(ft_strndup(*input, 2), get_ops_type(*input));
		(*input)++;
	}
	else
		token = ft_dlsnew(ft_strndup(*input, 1), get_ops_type(*input));
	(*input)++;
	return (token);
}

t_dls *tokenize_param(char **input)
{
	t_dls *token;
	int l;
	
	l = 0;
	if (**input == '"' || **input == '\"')
	{		
		l = loop_quote(input);
		if (l <= 0)
			printf("quote not closed");
	}
	else 
	{		
		while (!is_delim((*input) + l))
			l++;
	}
	token = ft_dlsnew(ft_strndup(*input, l), T_ARG);
	*input += l;
	return (token);
}

t_dls	*parse_token(char *input)
{
	t_dls *tokens = NULL;
	t_dls *token;

	while (*input)
	{
		ft_skipspaces(&input);
		if (get_ops_type(input))
			token = tokenize_operation(&input);
		else
			token = tokenize_param(&input);    
	    ft_dlsadd_back(&tokens, token);
	}
	return (tokens);
}