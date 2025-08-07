/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 15:26:28 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:38:14 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void printbanner(void)
{
    printf("\n");
    printf("███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     \n");
    printf("████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \n");
    printf("██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \n");
    printf("██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \n");
    printf("██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n");
    printf("╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝\n");
    printf("\n");
    printf("Welcome to Minishell! Type 'exit' to quit.\n\n");
}

void dbzSpinnerLoading(void)
{
    char spinner[] = "|/-\\";
    int i;
    
    printf("Loading");
    for (i = 0; i < 20; i++)
    {
        printf("\b%c", spinner[i % 4]);
        fflush(stdout);
        usleep(100000);
    }
    printf("\b \b");
}

void exitLoading(void)
{
    printf("Goodbye!\n");
    dbzSpinnerLoading();
}

size_t ft_strlen(const char *str)
{
    size_t len = 0;
    if (!str)
        return 0;
    while (str[len])
        len++;
    return len;
}

int ft_strcmp(const char *s1, const char *s2)
{
    if (!s1 || !s2)
        return (s1 == s2) ? 0 : (s1 ? 1 : -1);
        
    while (*s1 && *s2 && *s1 == *s2)
    {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}
