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

int	export(char **cmd, t_minishell **mnsh)
{
	t_envp	*temp;
	int		i;

	if (cmd[i][0] == '=')
		printf("%s not found", (cmd[i] + 1));
	if (ft_isdigit(cmd[i][0]))
		printf("export: not an identifier: %s", cmd[i]);
	
	else if (cmd)
	return (0);
}