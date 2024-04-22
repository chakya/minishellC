/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_string.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 09:45:00 by dphang            #+#    #+#             */
/*   Updated: 2024/04/18 09:45:12 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	init_parsestr(t_parsestr *pstr, char *str)
{
	pstr->i = 0;
	pstr->j = 0;
	pstr->sgl_quote = 0;
	pstr->dbl_quote = 0;
	pstr->temp = malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!pstr->temp)
		return ;
	pstr->temp[0] = '\0';
}

void	process_dollar(char *str, t_parsestr *pstr, t_minishell **mnsh)
{
	int	i;

	i = 1;
	pstr->temp[pstr->j] = '\0';
	apnd_expsn(str, pstr, mnsh);
	if (str[i] == '?')
		(pstr->i) += 2;
	else
	{
		(pstr->i)++;
		while (ft_isalnum(str[i]) || str[i] == '_')
		{
			i++;
			(pstr->i)++;
		}
	}
}

char	*cleanup_parse(t_parsestr *pstr)
{
	char	*res;

	pstr->temp[pstr->j] = '\0';
	res = ft_strdup(pstr->temp);
	free(pstr->temp);
	return (res);
}

char	*parse_string(char *str, t_minishell **mnsh)
{
	t_parsestr	pstr;

	init_parsestr(&pstr, str);
	while (str[pstr.i])
	{
		if (str[pstr.i] == '\'')
			parse_sglquote(&pstr, str);
		else if (str[pstr.i] == '"')
		{
			parse_dblquote(&pstr, str, mnsh);
		}
		else if (is_envar(str + (pstr.i)))
			process_dollar(str + (pstr.i), &pstr, mnsh);
		else
		{
			pstr.temp[pstr.j] = str[pstr.i];
			(pstr.j)++;
			(pstr.i)++;
		}
		if (pstr.sgl_quote == 1 || pstr.dbl_quote == 1)
			break ;
	}
	return (cleanup_parse(&pstr));
}

// char	*parse_string(char *str, t_minishell **mnsh)
// {
// 	int		i;
// 	int		j;
// 	char	*temp;
// 	char	*res;

// 	i = 0;
// 	j = 0;
// 	temp = malloc((ft_strlen(str) + 1) * sizeof(char));
// 	if (!temp)
// 		return (NULL);
// 	temp[0] = '\0';
// 	while (str[i])
// 	{
// 		if (str[i] == '\'')
// 		{
// 			i++;
// 			(*mnsh)->sgl_quote = 1;
// 			while (str[i] && str[i] != '\'')
// 			{
// 				temp[j] = str[i];
// 				j++;
// 				i++;
// 			}
// 			if (str[i] == '\'')
// 			{
// 				(*mnsh)->sgl_quote = 0;
// 				i++;
// 			}
// 		}
// 		else if (str[i] == '"')
// 		{
// 			i++;
// 			(*mnsh)->dbl_quote = 1;
// 			while (str[i] && str[i] != '"')
// 			{
// 				// check for envar
// 				if (is_envar(str + i))
// 				{
// 					temp[j] = '\0';
// 					apnd_expsn(&temp, str + i, mnsh, &j);
// 					if (str[i + 1] == '?')
// 						i += 2;
// 					else
// 					{
// 						i++;
// 						while (ft_isalnum(str[i]) || str[i] == '_')
// 						{
// 							i++;
// 						}
// 					}
// 				}
// 				else
// 				{
// 					temp[j] = str[i];
// 					j++;
// 					i++;
// 				}
// 			}
// 			if (str[i] == '"')
// 			{
// 				(*mnsh)->dbl_quote = 0;
// 				i++;
// 			}
// 		}
// 		// add check for envar
// 		else if (is_envar(str + i))
// 		{
// 			temp[j] = '\0';
// 			apnd_expsn(&temp, str + i, mnsh, &j);
// 			if (str[i + 1] == '?')
// 				i += 2;
// 			else
// 			{
// 				i++;
// 				while (ft_isalnum(str[i]) || str[i] == '_')
// 				{
// 					i++;
// 				}
// 			}
// 		}
// 		else 
// 		{
// 			temp[j] = str[i];
// 			j++;
// 			i++;
// 		}		
// 		if ((*mnsh)->sgl_quote == 1 || (*mnsh)->dbl_quote == 1)
// 		{
// 			(*mnsh)->sgl_quote = 0;
// 			(*mnsh)->dbl_quote = 0;
// 			break ;
// 		}
// 	}
// 	temp[j] = '\0';
// 	res = ft_strdup(temp);
// 	free(temp);
// 	return (res);
// }