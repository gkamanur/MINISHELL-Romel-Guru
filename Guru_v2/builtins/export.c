/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:31:13 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int is_valid_identifier(const char *str)
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

static void print_export_list(t_env *env_list)
{
    t_env *current = env_list;
    
    while (current)
    {
        if (current->key)
        {
            printf("declare -x %s", current->key);
            if (current->value)
                printf("=\"%s\"", current->value);
            printf("\n");
        }
        current = current->next;
    }
}

int builtin_export(t_tokens *tokens, t_env **env_list)
{
    t_tokens *current = tokens->next; // Skip "export"
    int status = 0;
    
    if (!current)
    {
        // No arguments, print all exported variables
        print_export_list(*env_list);
        return 0;
    }
    
    while (current)
    {
        char *equals = ft_strchr(current->token, '=');
        
        if (equals)
        {
            // Variable assignment
            *equals = '\0';
            char *key = current->token;
            char *value = equals + 1;
            
            if (is_valid_identifier(key))
            {
                set_env_variable(key, value, env_list);
            }
            else
            {
                printf("export: `%s': not a valid identifier\n", key);
                status = 1;
            }
            
            *equals = '='; // Restore original string
        }
        else
        {
            // Just export existing variable or create empty one
            if (is_valid_identifier(current->token))
            {
                char *existing_value = get_env_value(current->token, *env_list);
                if (!existing_value)
                    set_env_variable(current->token, "", env_list);
            }
            else
            {
                printf("export: `%s': not a valid identifier\n", current->token);
                status = 1;
            }
        }
        
        current = current->next;
    }
    
    return status;
}
// int builtin_export(t_tokens *tokens, t_env **env_list)
// {
//     char    **args;
//     char    *eq;
//     t_env   *node;
//     int     i = 1;

//     args = tokens_to_args(tokens);
//     if (!args)
//         return (1);
    
//     if (!args[1])
//     {
//         // No arguments: print the environment
//         for (t_env *cur = *env_list; cur; cur = cur->next)
//             printf("export %s=\"%s\"\n", cur->key, cur->value);
//     }
//     else
//     {
//         // Arguments provided: add/update variables
//         while (args[i])
//         {
//             eq = strchr(args[i], '=');
//             if (eq)
//             {
//                 // Temporarily split the string
//                 *eq = '\0';
                
//                 // Check if the variable already exists and update it
//                 t_env *cur = *env_list;
//                 while (cur)
//                 {
//                     if (strcmp(cur->key, args[i]) == 0)
//                     {
//                         free(cur->value);
//                         cur->value = strdup(eq + 1);
//                         *eq = '='; // Restore the string
//                         break;
//                     }
//                     cur = cur->next;
//                 }

//                 // If not found, create a new node
//                 if (!cur)
//                 {
//                     node = create_env_node(strdup(args[i]), strdup(eq + 1));
//                     if (!node)
//                     {
//                         free(args);
//                         *eq = '='; // Restore the string before returning
//                         return (1);
//                     }
//                     node->next = *env_list;
//                     *env_list = node;
//                 }
//                 *eq = '='; // Restore the string
//             }
//             else
//             {
//                 // Handle cases like 'export MYVAR' without a value
//                 fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", args[i]);
//                 // Depending on the desired behavior, you might return 1 here or continue
//             }
//             i++;
//         }
//     }
//     free(args);
//     return (0);
// }
