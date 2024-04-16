/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <dphang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:02:40 by dphang            #+#    #+#             */
/*   Updated: 2024/04/16 16:59:03 by dphang           ###   ########.fr       */
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

void	add_exp(char *envp, t_minishell **mnsh)
{
	t_envp	*temp;
	t_envp	*undsc_temp;

	if (ft_strchr(envp, '='))
	{
		temp = (*mnsh)->envp;
		while (ft_strncmp(temp->next->content, "_=", 2) != 0)
		{
			temp = temp->next;
		}
		undsc_temp = temp->next;
		temp->next = newenvp(envp);
		temp->next->next = undsc_temp;
	}
}

char	*enclose_val(char *envp)
{
	int		i;
	int		j;
	char	*res;
	int		equal;

	i = 0;
	j = 0;
	equal = 0;
	res = malloc((ft_strlen(envp) + 3) * sizeof(char));
	while (envp[i])
	{
		if (envp[i] == '=' && equal == 0)
		{
			res[j++] = envp[i++];
			res[j++] = '"';
			equal = 1;
		}
		else
			res[j++] = envp[i++];
	}
	res[j] = '"';
	res[j + 1] ='\0';
	return (res);
}

t_envp	*dup_envp(t_envp *envp)
{
	t_envp	*dup;
	t_envp	*dup_temp;
	t_envp	*temp;
	char	*str;

	str = enclose_val(envp->content);
	dup = newenvp(str);
	free(str);
	dup_temp = dup;
	temp = envp->next;
	while (temp && temp->next != NULL)
	{
		if (ft_strncmp(temp->content, "_=", 2) != 0)
		{
			str = enclose_val(temp->content);
			dup_temp->next = newenvp(str);
			free(str);
			dup_temp = dup_temp->next;
		}
		temp = temp->next;
	}
	return (dup);
}

void	swap_envp(t_envp **dup, t_envp *sa, t_envp *sb)
{
	t_envp	*prev;
	t_envp	*temp;

	prev = NULL;
	temp = *dup;
	while (temp && temp != sa)
	{
		prev = temp;
		temp = temp->next;
	}
	if (!prev)
		*dup = sb;
	else
		prev->next = sb;
	sa->next = sb->next;
	sb->next = sa;
}

void	sort_print(t_envp *envp)
{
	t_envp	*dup;
	t_envp	*end;
	t_envp	*temp;
	int		i;
	int		diff;

	dup = dup_envp(envp);
	end = NULL;
	temp = dup;
	while (temp != end)
	{
		while (temp->next != end)
		{
			i = 0;
			diff = 0;
			while (diff == 0 && temp->content[i] && temp->next->content[i])
			{
				diff = (temp->content[i] + ft_isdigit(temp->content[i]))
					- (temp->next->content[i]
					+ ft_isdigit(temp->next->content[i]));
				i++;
			}
			if (diff > 0)
			{
				swap_envp(&dup, temp, temp->next);
				temp = dup;
			}
			else
				temp = temp->next;
		}
		if (temp->next == end)
		{
			end = temp;
			temp = dup;
		}
	}
	while (dup)
	{
		temp = dup;
		dup = dup->next;
		printf("declare -x %s\n", temp->content);
		free(temp->content);
		free(temp);
	}
}

int	export(char **cmd, t_minishell **mnsh)
{
	t_envp	*var;
	int		exit_code;
	int		i;

	var = NULL;
	i = 1;
	exit_code = 0;
	if (!cmd[i])
		sort_print((*mnsh)->envp);
	while (cmd[i])
	{
		if (cmd[i][0] == '=' || (!(ft_isalpha(cmd[i][0]) || cmd[i][0] == '_')))
		{
			if (cmd[i][0] == '=')
				printf("%s not found\n", (cmd[i] + 1));
			if (!(ft_isalpha(cmd[i][0]) || cmd[i][0] == '_'))
				printf("export: '%s': not a valid identifier\n", cmd[i]);
			exit_code = 1;
		}
		else
		{
			var = envp_exist(cmd[i], mnsh);
			if (var)
			{
				free(var->content);
				var->content = ft_strdup(cmd[i]);
			}
			else
				add_exp(cmd[i], mnsh);
		}
		i++;
	}
	return (exit_code);
}
