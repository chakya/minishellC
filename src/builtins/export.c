/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:02:40 by dphang            #+#    #+#             */
/*   Updated: 2024/04/23 11:43:58 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	print_exporterr(char *cmd)
{
	ft_putstr_fd("export: '", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd("' not a valid identifier\n", 2);
	return (1);
}

void	check_envp(t_envp **var, char *cmd, t_minishell **mnsh)
{
	*var = envp_exist(cmd, mnsh);
	if (*var)
	{
		free((*var)->content);
		(*var)->content = ft_strdup(cmd);
	}
	else
		add_exp(cmd, mnsh);
}

int	export(char **cmd, t_minishell **mnsh)
{
	t_envp	*var;
	int		exit_code;
	int		i;

	var = NULL;
	i = 1;
	exit_code = 0;
	if (!cmd[i] || cmd[i][0] == '#')
		sort_print((*mnsh)->envp);
	while (cmd[i] && cmd[i][0] != '#')
	{
		if (!is_validenvar(cmd[i]))
			exit_code = print_exporterr(cmd[i]);
		else
		{
			if (ft_strchr(cmd[i], '='))
				check_envp(&var, cmd[i], mnsh);
		}
		i++;
	}
	return (exit_code);
}
