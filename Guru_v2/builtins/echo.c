/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:32:04 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int builtin_echo(t_tokens *tokens, t_env **env_list)
{
    t_tokens *current = tokens->next; // Skip "echo"
    int newline = 1;
    int first_arg = 1;
    
    // Check for -n flag
    if (current && current->token && ft_strcmp(current->token, "-n") == 0)
    {
        newline = 0;
        current = current->next;
    }
    
    while (current)
    {
        if (!first_arg)
            printf(" ");
            
        // Handle variable expansion and quote processing
        if (current->type == TOKEN_VARIABLE)
        {
            char *var_value = expand_variable(current->token, *env_list);
            if (var_value)
            {
                printf("%s", var_value);
                Free(var_value, "echo_var");
            }
        }
        else if (current->type == TOKEN_DQUOTED)
        {
            char *processed = process_quotes_and_vars(current->token, *env_list);
            printf("%s", processed);
            Free(processed, "echo_dquote");
        }
        else
        {
            printf("%s", current->token);
        }
        
        first_arg = 0;
        current = current->next;
    }
    
    if (newline)
        printf("\n");
        
    return 0;
}
// int builtin_echo(t_tokens *tokens, t_env **env_list)
// {
    
//     (void) env_list;
//     char    **args;
//     int     no_newline = 0;
//     int     i = 1;

//     // The function receives a single pointer to the head of the list
//     args = tokens_to_args(tokens);
//     if (!args)
//         return (1);

//     if (args[1] && strcmp(args[1], "-n") == 0)
//     {
//         no_newline = 1;
//         i++;
//     }

//     while (args[i])
//     {
//         printf("%s", args[i]);
//         if (args[i + 1])
//             printf(" ");
//         i++;
//     }

//     if (!no_newline)
//         printf("\n");

//     free(args); // Don't free the strings, just the array itself
//     return (0);
// }