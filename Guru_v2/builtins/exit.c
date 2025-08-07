/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:31:36 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_numeric(const char *str)
{
    int i = 0;
    
    if (!str)
        return 0;
        
    if (str[0] == '-' || str[0] == '+')
        i = 1;
        
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    
    return 1;
}

void builtin_exit(t_tokens *tokens, t_env **env_list, char *inp_line)
{
    int exit_code = g_status;
    
    printf("exit\n");
    
    if (tokens->next)
    {
        if (tokens->next->next)
        {
            putstr_fd("exit: too many arguments\n", 2);
            g_status = 1;
            return; // Don't exit, just return error
        }
        
        if (!is_numeric(tokens->next->token))
        {
            putstr_fd("exit: numeric argument required\n", 2);
            exit_code = 2;
        }
        else
        {
            exit_code = atoi(tokens->next->token);
        }
    }
    
    // Cleanup
    if (inp_line)
        Free(inp_line, "exit_inp_line");
    if (env_list && *env_list)
        free_env(*env_list, "exit_env");
    rl_clear_history();
    
    exitLoading();
    check_memory_leaks();
    exit(exit_code);
}
// void builtin_exit(t_tokens *tokens, t_env **env_list, char *inp_line)
// {
//     (void) env_list;
//     (void) inp_line;
//     dbzSpinnerLoading(); // Optional animation/effect

//     free_tokens(tokens, "builtin_exit_tokens");
    
//     // if (inp_line)
//     //     Free(inp_line, "builtin_exit_input_line");

//     // if (env_list && *env_list)
//     //     free_env(*env_list, "builtin_exit_env");

//     check_memory_leaks();
//     exit(0);
// }
