/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:58:22 by dphang            #+#    #+#             */
/*   Updated: 2024/04/18 22:26:52 by cwijaya          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	envar_exist(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
			return (1);
		else if (str[i] == '$' && str[i + 1] == '?')
			return (2);
		i++;
	}
	return (0);
}

int	is_envar(char *str)
{
	if (str[0] == '$'
		&& (ft_isalnum(str[1]) || str[1] == '_' || str[1] == '?'))
		return (1);
	return (0);
}

char	*enval(char *var, t_envp *envp)
{
	while (envp && ft_strncmp(var, envp->content, ft_strlen(var)) != 0)
	{
		envp = envp->next;
	}
	if (envp)
		return (envp->content + ft_strlen(var));
	else
		return ("");
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
	name = malloc((j + 2) * sizeof(char));
	if (name == NULL)
		return (NULL);
	while (i < j)
	{
		name[i] = str[i];
		i++;
	}
	name[i++] = '=';
	name[i] = '\0';
	var_val = enval(name, envp);
	free(name);
	return (var_val);
}

// char	*parse_dollar(char *str, t_minishell **mnsh)
// {
// 	char	*temp;
// 	char	*temp_res;
// 	char	*res;
// 	int		i;
// 	int		j;

// 	i = 0;
// 	temp = malloc((ft_strlen(str) + 1) * sizeof(char));
// 	res = NULL;
// 	while (str[i])
// 	{
// 		j = 0;
// 		while (!(str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?')))
// 		{
// 			temp[j] = str[i];
// 			i++;
// 			j++;
// 		}
// 		temp[j] = '\0';
// 		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_' || str[i + 1] == '?'))
// 		{
// 			i++;
// 			if (ft_isalnum(str[i]) || str[i] == '_')
// 			{
// 				temp_res = ft_strdup(temp);
// 				free(temp);
// 				res = ft_strjoin(temp_res, get_enval(str + i, (*mnsh)->envp));
// 				free(temp_res);
// 				while ((ft_isalnum(str[i]) || str[i] == '_' || str[i] == '?'))
// 				{
// 					i++;
// 				}
// 			}
// 			else if (str[i] == '?')
// 			{
// 				temp_res = ft_strdup(temp);
// 				free(temp);
// 				temp = ft_strjoin(temp_res, ft_itoa((*mnsh)->exit_code));
// 				free(temp_res);
// 				i++;
// 			}
// 			if (res == NULL)
// 				res = ft_strdup(temp);
// 			else
// 			{
// 				temp_res = ft_strdup(res);
// 				free(res);
// 				res = ft_strjoin(temp_res, temp);
// 				free(temp);
// 				free(temp_res);
// 				temp = malloc((ft_strlen(str + i) + 1) * sizeof(char));
// 			}
// 		}
// 	}
// 	return (res);
// }

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

// void	apnd_expsn(char **temp, char *str, t_minishell **mnsh, int *j)
// {
// 	char	*temp_dup;
// 	char	*temp_val;

// 	temp_dup = ft_strdup(*temp);
// 	temp_val = expand_dollar(str, mnsh);
// 	*j += ft_strlen(temp_val);
// 	free(*temp);
// 	*temp = malloc((ft_strlen(temp_dup) + ft_strlen(temp_val)
// 		+ ft_strlen(str) + 1) * sizeof(char));
// 	join_expsn(temp, temp_dup, temp_val);
// 	free(temp_dup);
// 	free(temp_val);
// }
