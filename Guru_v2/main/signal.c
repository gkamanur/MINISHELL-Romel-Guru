/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 11:56:18 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:29:46 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int g_status = 0;
char *g_user_input = NULL;
int g_quit = 0;

void handle_sigint(int sig)
{
    (void)sig;
    g_status = 130;
    g_quit = 1;
    
    printf("\n");
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void handle_sigquit(int sig)
{
    (void)sig;
    // Do nothing for SIGQUIT in interactive mode
}

void init_signals(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);
}

void log_debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    
    printf("[DEBUG] ");
    vprintf(fmt, args);
    printf("\n");
    
    va_end(args);
}
