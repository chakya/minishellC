/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mnsh_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dphang <dphang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:01:39 by dphang            #+#    #+#             */
/*   Updated: 2024/04/03 23:35:00 by dphang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*
	Implement redirections:
		<	: redirect input
		>	: redirect output
		<<	: should be given a delimited, then read the input until a line
			  containing the delimiter is seen. Doesn't have to update the
			  history.
		>>	: redirect output in append mode

	Implement pipes:
		|	: output of each command in the pipeline is connected to the input
			  of the next command via a pipe
*/

// int	main(int ac, char **av, char **envp)
// {
// 	(void)ac;
// 	pid_t	id;
// 	int		fd[2];
// 	char	*cmd;
// 	int		i;

// 	pipe(fd);
// 	id = fork();

// 	if (id = 0)
// 	{
// 		// child process
// 	}
// 	else
// 	{
// 		// parent process
// 	}
// 	return (0);
// }

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	pid_t	id;
	int		fd[2];
	char	*cmd;
	int		i;

	pipe(fd);
	id = fork();

	if (av > 1)
	{
		if (id = 0)
		{
			// child process
		}
		else
		{
			// parent process
		}
	}
	return (0);
}