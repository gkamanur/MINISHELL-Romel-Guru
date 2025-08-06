/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/05 11:46:16 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void builtin_exit(t_tokens *tokens, t_env **env_list, char *inp_line)
{
    (void) env_list;
    (void) inp_line;
    dbzSpinnerLoading(); // Optional animation/effect

    free_tokens(tokens, "builtin_exit_tokens");
    
    // if (inp_line)
    //     Free(inp_line, "builtin_exit_input_line");

    // if (env_list && *env_list)
    //     free_env(*env_list, "builtin_exit_env");

    check_memory_leaks();
    exit(0);
}
