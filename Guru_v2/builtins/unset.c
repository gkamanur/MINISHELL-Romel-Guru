/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:30:39 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_valid_unset_identifier(const char *str)
{
    int i = 0;
    
    if (!str || !str[0])
        return 0;
        
    if (!ft_isalpha(str[0]) && str[0] != '_')
        return 0;
        
    while (str[i])
    {
        if (!ft_isalpha(str[i]) && str[i] != '_' && 
            !(str[i] >= '0' && str[i] <= '9'))
            return 0;
        i++;
    }
    
    return 1;
}

int builtin_unset(t_tokens *tokens, t_env **env_list)
{
    t_tokens *current = tokens->next; // Skip "unset"
    int status = 0;
    
    if (!current)
    {
        putstr_fd("unset: not enough arguments\n", 2);
        return 1;
    }
    
    while (current)
    {
        if (is_valid_unset_identifier(current->token))
        {
            unset_env_variable(current->token, env_list);
        }
        else
        {
            printf("unset: `%s': not a valid identifier\n", current->token);
            status = 1;
        }
        
        current = current->next;
    }
    
    return status;
}
// #include <stdlib.h>
// #include <string.h>
// #include <stdio.h> // For debugging

// int builtin_unset(t_tokens *tokens, t_env **env_list)
// {
//     t_tokens *current_token;
//     t_env    *current_env;
//     t_env    *prev_env;
//     t_env *temp_env = *env_list;

//     // Debug: Confirm function is called
//     printf("builtin_unset called\n");

//     // Check for valid token list and environment list
//     if (!tokens || !env_list)
//     {
//         fprintf(stderr, "builtin_unset: Invalid tokens or env_list\n");
//         return 1;
//     }

//     // Debug: Print token list
//     printf("Tokens:\n");
//     t_tokens *temp_token = tokens;
//     while (temp_token)
//     {
//         printf("  Token: %s\n", temp_token->token ? temp_token->token : "NULL");
//         temp_token = temp_token->next;
//     }

//     // Start from the second token (skip "unset")
//     current_token = tokens->next;

//     while (current_token)
//     {
//         // Validate token content
//         if (!current_token->token || !*current_token->token)
//         {
//             fprintf(stderr, "builtin_unset: Skipping invalid token\n");
//             current_token = current_token->next;
//             continue;
//         }

//         // Debug: Print environment list before unset
//         printf("Environment before unsetting '%s':\n", current_token->token);
        
//         while (temp_env)
//         {
//             printf("  %s=%s\n", temp_env->key, temp_env->value);
//             temp_env = temp_env->next;
//         }

//         current_env = *env_list;
//         prev_env = NULL;

//         // Search for the environment variable
//         while (current_env)
//         {
//             // Debug: Print comparison
//             printf("Comparing: token='%s' with env_key='%s'\n",
//                    current_token->token, current_env->key);
//             if (current_env->key && strcmp(current_env->key, current_token->token) == 0)
//             {
//                 // Found the variable to unset
//                 if (prev_env)
//                     prev_env->next = current_env->next;
//                 else
//                     *env_list = current_env->next;

//                 // Free the node's contents
//                 if (current_env->key)
//                     free(current_env->key);
//                 if (current_env->value)
//                     free(current_env->value);
//                 free(current_env);
//                 printf("Unset '%s' successfully\n", current_token->token);
//                 break;
//             }
//             prev_env = current_env;
//             current_env = current_env->next;
//         }
//         current_token = current_token->next;
//     }

//     // Debug: Print environment list after unset
//     printf("Environment after unset:\n");
//     temp_env = *env_list;
//     while (temp_env)
//     {
//         printf("  %s=%s\n", temp_env->key, temp_env->value);
//         temp_env = temp_env->next;
//     }

//     return 0; // Success
// }
