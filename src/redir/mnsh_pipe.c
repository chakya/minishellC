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

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	// pid_t	pid;
	char	**path;
	// int		fd[2];
	char	*cmd;
	int		i;

	// if (pipe(fd) == -1)
	// 	return (1);
	path = get_path(envp);
	// pid = fork();

	// if (pid == 0)
	// {
	// 	i = 0;
	// 	// Child process
	// 	close(fd[0]);
	// 	dup2(fd[1], 1);
	// 	close(fd[1]);
	// 	while (execve(path[i], av, envp) == -1)
	// 		i++;
	// 	printf("Child process\n");
	// }
	// else if (pid > 0)
	// {
	// 	i = 0;
	// 	// Parent process
	// 	close(fd[1]);
	// 	dup2(fd[0], 0);
	// 	close(fd[0]);
	// 	while (execve(path[i], av, envp) == -1)
	// 		i++;
	// 	printf("Parent process\n");
	// 	waitpid(pid, NULL, 0);
	// }

	i = 0;
	cmd = ft_strjoin(path[i], av[1]);
	while (execve(cmd, av + 2, envp) == -1)
	{
		free(cmd);
		i++;
		cmd = ft_strjoin(path[i], av[1]);
	}
	free(cmd);
	i = 0;
	while (path[i])
	{
		free(path[i]);
		i++;
	}
	return (0);
}