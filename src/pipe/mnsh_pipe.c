/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mnsh_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwijaya <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/03 14:01:39 by dphang            #+#    #+#             */
/*   Updated: 2024/04/04 23:57:21 by cwijaya          ###   ########.fr       */
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

int	main(int ac, char **av, char **envp)
{
	pid_t	pid;
	char	**path;
	int		fd[2];

	if (pipe(fd) == -1)
		return (1);
	path = get_path(envp);
	pid = fork();

	if (pid == 0)
	{
		// Child process
		close(fd[0]);
		dup2(fd[1], 1);
		close(fd[1]);
		execve();
		printf("Child process\n");
	}
	else if (pid > 0)
	{
		// Parent process
		close(fd[1]);
		dup2(fd[0], 0);
		close(fd[0]);
		execve();
		printf("Parent process\n");
		waitpid(pid, NULL, 0);
	}
	

	return (0);
}
