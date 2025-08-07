/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:03:51 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/05 09:48:18 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	putstr_fd(char *str, int fd)
{
	while (*str)
		write(fd, str++, 1);
}

void	putstr_clr(char *str, int fd, char *clr)
{
	putstr_fd(clr, fd);
	putstr_fd(str, fd);
	putstr_fd(RESET "\n", fd);
}

pid_t	Fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		perror(RED"Fork failed"RESET);
		exit(EX_OSERR);
	}
	return (pid);
}


void Chdir(const char *path)
{
    if (!path)
        putstr_clr("cd: give the path",2, RED);
    else if (chdir(path) == -1)
       putstr_clr("cd FAILED",2, RED);
    return ;
}
