/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:37:05 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 15:53:19 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// void handle_sigint(int sig)
// {
//     (void)sig;
//     printf("\n");
//     rl_on_new_line();
//     rl_replace_line("", 0);
//     rl_redisplay();
//     g_status = 130;
// }

// void handle_sigquit(int sig)
// {
//     (void)sig;
//     // Ignore SIGQUIT in interactive mode
// }

// void init_signals(void)
// {
//     signal(SIGINT, handle_sigint);
//     signal(SIGQUIT, handle_sigquit);
// }

// void sig_exec_init(void)
// {
//     signal(SIGINT, SIG_DFL);
//     signal(SIGQUIT, SIG_DFL);
// }

// // Placeholder functions for compatibility
// int var_index(char *var, t_cmd *data)
// {
//     (void)var;
//     (void)data;
//     return -1;
// }

// char **gen_paths(int index, t_cmd *data, char *cmd)
// {
//     (void)index;
//     (void)data;
//     (void)cmd;
//     return NULL;
// }

// int check_exec(char **inputs, t_cmd *data)
// {
//     (void)inputs;
//     (void)data;
//     return 1;
// }

// void error_sentence(char *msg, int code)
// {
//     putstr_clr(msg, 2, RED);
//     g_status = code;
// }

// void handle_exec(char **inputs, t_cmd *data)
// {
//     (void)inputs;
//     (void)data;
// }
