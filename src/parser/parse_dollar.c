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
	while (str[i])
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
	while (ft_strncmp(var, envp->content, ft_strlen(var)) != 0)
	{
		envp = envp->next;
	}
	return (envp->content + ft_strlen(var));
}

char	*get_enval(char *str, t_envp *envp)
{
	int		i;
	int		j;
	char	*name;
	char	*var_val;

	i = 0;
	j = ft_strlen(str);
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

char	*get_val(char *str, t_minishell **mnsh)
{
	char	*res;

	if (ft_isalnum(str[1]) || str[1] == '_')
		res = ft_strdup(get_enval(str + 1, (*mnsh)->envp));
	else if (str[1] == '?')
		res = ft_strdup(ft_itoa((*mnsh)->exit_code));
	return (res);
}

// void	put_envar(char *str, t_minishell **mnsh)
// {
// 	int	i;

// 	i = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '$' && (ft_isalnum(str[i + 1]) || str[i + 1] == '_'))
// 			put_enval(str, &i, (*mnsh)->envp);
// 		else if (str[i] == '$' && str[i + 1] == '?')
// 		{
// 			printf("%d", (*mnsh)->exit_code);
// 			i += 2;
// 		}
// 		else
// 		{
// 			ft_putchar_fd(str[i], 1);
// 			i++;
// 		}
// 	}
// }
