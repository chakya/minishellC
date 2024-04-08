/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mnsh_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <dphang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:02:40 by dphang            #+#    #+#             */
/*   Updated: 2024/04/04 16:59:29 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_envp	*envp_exist(char *envp, t_minishell **mnsh)
{
	int		var;
	t_envp	*temp;

	var = 0;
	while (envp[var])
	{
		var++;
		if (envp[var] == '=')
			break;
		else if (envp[var] != '=' && envp[var + 1] == '\0')
			return (NULL);
	}
	temp = (*mnsh)->envp;
	while (temp)
	{
		if (ft_strncmp(temp->content, envp, var) == 0)
			return (temp);
		temp = temp->next;
	}
	return (NULL);
}

void	update_exp(char *envp, t_envp *upd)
{
	free(upd->content);
	upd->content = ft_strdup(envp);
}

void	add_exp(char *envp, t_minishell **mnsh)
{
	t_envp	*temp;
	t_envp	*undsc_temp;
	
	temp = (*mnsh)->envp;
	while (ft_strncmp(temp->next->content, "_=", 2) != 0)
	{
		temp = temp->next;
	}
	undsc_temp = temp->next;
	temp->next = newenvp(envp);
	temp->next->next = undsc_temp;
}

int	export(char **cmd, t_minishell **mnsh)
{
	t_envp	*var;
	int		i;

	i = 1;
	while (cmd[i])
	{
		if (cmd[i][0] == '=')
			printf("%s not found\n", (cmd[i] + 1));
		if (!(ft_isalpha(cmd[1][0]) || cmd[1][0] == '_'))
			printf("export: '%s': not a valid identifier\n", cmd[i]);
		var = envp_exist(cmd[i], mnsh);
		if (var)
			update_exp(cmd[i], var);
		else
			add_exp(cmd[i], mnsh);
		i++;
	}
	return (0);
}