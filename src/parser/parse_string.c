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

char	*parse_string(char *str, t_minishell **mnsh)
{
	int		i;
	int		j;
	char	*temp;
	char	*res;

	i = 0;
	j = 0;
	temp = malloc((ft_strlen(str) + 1) * sizeof(char));
	if (!temp)
		return (NULL);
	temp[0] = '\0';
	while (str[i])
	{
		if (str[i] == '\'')
		{
			i++;
			(*mnsh)->sgl_quote = 1;
			while (str[i] && str[i] != '\'')
			{
				temp[j] = str[i];
				j++;
				i++;
			}
			if (str[i] == '\'')
			{
				(*mnsh)->sgl_quote = 0;
				i++;
			}
		}
		else if (str[i] == '"')
		{
			i++;
			(*mnsh)->dbl_quote = 1;
			while (str[i] && str[i] != '"')
			{
				// check for envar
				if (is_envar(str + i))
				{
					temp[j] = '\0';
					apnd_expsn(&temp, str + i, mnsh, &j);
					if (str[i + 1] == '?')
						i += 2;
					else
					{
						i++;
						while (ft_isalnum(str[i]) || str[i] == '_')
						{
							i++;
						}
					}
				}
				else
				{
					temp[j] = str[i];
					j++;
					i++;
				}
			}
			if (str[i] == '"')
			{
				(*mnsh)->dbl_quote = 0;
				i++;
			}
		}
		// add check for envar
		else if (is_envar(str + i))
		{
			temp[j] = '\0';
			apnd_expsn(&temp, str + i, mnsh, &j);
			if (str[i + 1] == '?')
				i += 2;
			else
			{
				i++;
				while (ft_isalnum(str[i]) || str[i] == '_')
				{
					i++;
				}
			}
		}
		else 
		{
			temp[j] = str[i];
			j++;
			i++;
		}		
		if ((*mnsh)->sgl_quote == 1 || (*mnsh)->dbl_quote == 1)
		{
			(*mnsh)->sgl_quote = 0;
			(*mnsh)->dbl_quote = 0;
			break ;
		}
	}
	temp[j] = '\0';
	res = ft_strdup(temp);
	free(temp);
	return (res);
}
