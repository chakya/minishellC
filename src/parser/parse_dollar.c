/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_dollar.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 10:58:22 by dphang            #+#    #+#             */
/*   Updated: 2024/04/17 11:00:29 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_envar(char *str)
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

char	*parse_dollar(char *str, t_minishell **mnsh)
{
	char	*res;

	if (ft_isalnum(str[1]) || str[1] == '_')
		res = ft_strdup(get_enval(str + 1, (*mnsh)->envp));
	else if (str[1] == '?')
		res = ft_strjoin(ft_itoa((*mnsh)->exit_code), str + 2);
	return (res);
}