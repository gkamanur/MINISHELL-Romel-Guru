/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:03:51 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:38:32 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void putstr_fd(char *str, int fd)
{
    if (!str)
        return;
    write(fd, str, ft_strlen(str));
}

void putstr_clr(char *str, int fd, char *clr)
{
    if (!str || !clr)
        return;
    putstr_fd(clr, fd);
    putstr_fd(str, fd);
    putstr_fd("\033[0m", fd); // Reset color
}

pid_t Fork(void)
{
    pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    return pid;
}

pid_t Waitpid(pid_t pid, int *status, int options)
{
    pid_t result = waitpid(pid, status, options);
    if (result == -1)
    {
        perror("waitpid");
        exit(EXIT_FAILURE);
    }
    return result;
}

char *Readline(void)
{
    char *line = readline("minishell> ");
    if (line && *line)
        add_history(line);
    return line;
}
