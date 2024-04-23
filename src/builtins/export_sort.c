/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_sort.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 11:42:09 by dphang            #+#    #+#             */
/*   Updated: 2024/04/23 12:02:21 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

void	check_diff(int diff, t_envp **temp, t_envp **dup)
{
	if (diff > 0)
	{
		swap_envp(dup, *temp, (*temp)->next);
		*temp = *dup;
	}
	else
		*temp = (*temp)->next;
}

void	envp_sort(t_envp **dup, t_envp **end, t_envp **temp)
{
	int	i;
	int	diff;

	while ((*temp)->next != *end)
	{
		i = 0;
		diff = 0;
		while (diff == 0 && (*temp)->content[i] && (*temp)->next->content[i])
		{
			diff = ((*temp)->content[i] + ft_isdigit((*temp)->content[i]))
				- ((*temp)->next->content[i]
					+ ft_isdigit((*temp)->next->content[i]));
			i++;
		}
		check_diff(diff, temp, dup);
	}
	if ((*temp)->next == *end)
	{
		*end = *temp;
		*temp = *dup;
	}
}

void	sort_print(t_envp *envp)
{
	t_envp	*dup;
	t_envp	*end;
	t_envp	*temp;

	dup = dup_envp(envp);
	end = NULL;
	temp = dup;
	while (temp != end)
	{
		envp_sort(&dup, &end, &temp);
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
