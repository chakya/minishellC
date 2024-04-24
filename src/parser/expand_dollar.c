/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:58:22 by dphang            #+#    #+#             */
/*   Updated: 2024/04/24 17:45:50 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*get_envname(char *str, int *i, int j)
{
	char	*name;

	name = malloc((j + 2) * sizeof(char));
	if (name == NULL)
		return (NULL);
	while ((*i) < j)
	{
		name[*i] = str[*i];
		(*i)++;
	}
	name[*i] = '=';
	name[(*i) + 1] = '\0';
	return (name);
}

char	*get_enval(char *str, t_envp *envp)
{
	int		i;
	int		j;
	char	*name;
	char	*var_val;

	i = 0;
	j = 1;
	while (str[j])
	{
		if (!(ft_isalnum(str[j]) || str[j] == '_'))
			break ;
		j++;
	}
	name = get_envname(str, &i, j);
	var_val = enval(name, envp);
	free(name);
	return (var_val);
}

char	*expand_dollar(char *str, t_minishell **mnsh)
{
	char	*res;

	res = NULL;
	if (ft_isalnum(str[1]) || str[1] == '_')
		res = ft_strdup(get_enval(str + 1, (*mnsh)->envp));
	else if (str[1] == '?')
		res = ft_strdup(ft_itoa((*mnsh)->exit_code));
	return (res);
}

void	join_expsn(char **dst, char *s1, char *s2)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s1[i])
	{
		(*dst)[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2[i])
	{
		(*dst)[j] = s2[i];
		i++;
		j++;
	}
	(*dst)[j] = '\0';
}

void	apnd_expsn(char *str, t_parsestr *pstr, t_minishell **mnsh)
{
	char	*temp_dup;
	char	*temp_val;

	temp_dup = ft_strdup(pstr->temp);
	temp_val = expand_dollar(str, mnsh);
	pstr->j += ft_strlen(temp_val);
	free(pstr->temp);
	pstr->temp = malloc((ft_strlen(temp_dup) + ft_strlen(temp_val)
				+ ft_strlen(str) + 1) * sizeof(char));
	join_expsn(&(pstr->temp), temp_dup, temp_val);
	free(temp_dup);
	free(temp_val);
}
