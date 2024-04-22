/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 09:43:44 by dphang            #+#    #+#             */
/*   Updated: 2024/04/18 09:44:06 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	parse_sglquote(t_parsestr *pstr, char *str)
{
	(pstr->i)++;
	pstr->sgl_quote = 1;
	while (str[pstr->i] && str[pstr->i] != '\'')
	{
		pstr->temp[pstr->j] = str[pstr->i];
		(pstr->j)++;
		(pstr->i)++;
	}
	if (str[pstr->i] == '\'')
	{
		pstr->sgl_quote = 0;
		(pstr->i)++;
	}
}

void	parse_dblquote(t_parsestr *pstr, char *str, t_minishell **mnsh)
{
	(pstr->i)++;
	pstr->dbl_quote = 1;
	while (str[pstr->i] && str[pstr->i] != '"')
	{
		if (is_envar(str + (pstr->i)))
			process_dollar(str + (pstr->i), pstr, mnsh);
		else
		{
			pstr->temp[pstr->j] = str[pstr->i];
			(pstr->j)++;
			(pstr->i)++;
		}
	}
	if (str[pstr->i] == '"')
	{
		pstr->dbl_quote = 0;
		(pstr->i)++;
	}
}
