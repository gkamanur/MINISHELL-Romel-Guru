/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 10:06:19 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:48:26 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

char *expand_variable(const char *var_name, t_env *env_list)
{
    if (!var_name || !env_list)
        return NULL;
        
    // Handle special variables
    if (ft_strcmp(var_name, "$?") == 0)
    {
        char *status_str = Malloc(12, "status_expand");
        sprintf(status_str, "%d", g_status);
        return status_str;
    }
    
    if (ft_strcmp(var_name, "$$") == 0)
    {
        char *pid_str = Malloc(12, "pid_expand");
        sprintf(pid_str, "%d", getpid());
        return pid_str;
    }
    
    // Skip $ if present
    if (var_name[0] == '$')
        var_name++;
        
    return ft_strdup(get_env_value(var_name, env_list), "var_expand");
}

char *process_quotes_and_vars(const char *input, t_env *env_list)
{
    char *result = Malloc(strlen(input) * 2 + 1, "quote_process");
    int i = 0, j = 0;
    int in_single_quotes = 0;
    int in_double_quotes = 0;
    
    while (input[i])
    {
        if (input[i] == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            i++;
            continue;
        }
        
        if (input[i] == '"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            i++;
            continue;
        }
        
        if (input[i] == '$' && !in_single_quotes)
        {
            // Variable expansion
            i++; // Skip $
            char var_name[256] = {0};
            int k = 0;
            
            while (input[i] && (ft_isalpha(input[i]) || input[i] == '_' || 
                   (k > 0 && input[i] >= '0' && input[i] <= '9')))
            {
                var_name[k++] = input[i++];
            }
            
            if (k > 0)
            {
                char *var_value = get_env_value(var_name, env_list);
                if (var_value)
                {
                    strcpy(result + j, var_value);
                    j += strlen(var_value);
                }
            }
            else
            {
                result[j++] = '$'; // Keep $ if no valid variable name follows
            }
        }
        else
        {
            result[j++] = input[i++];
        }
    }
    
    result[j] = '\0';
    return result;
}

void set_env_variable(const char *key, const char *value, t_env **env_list)
{
    t_env *current = *env_list;
    
    // Check if variable already exists
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            Free(current->value, "set_env_old_value");
            current->value = ft_strdup(value, "set_env_new_value");
            return;
        }
        current = current->next;
    }
    
    // Create new variable
    t_env *new_var = create_env_node(key, value);
    new_var->next = *env_list;
    *env_list = new_var;
}

void unset_env_variable(const char *key, t_env **env_list)
{
    t_env *current = *env_list;
    t_env *prev = NULL;
    
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                *env_list = current->next;
                
            Free(current->key, "unset_key");
            Free(current->value, "unset_value");
            Free(current, "unset_node");
            return;
        }
        prev = current;
        current = current->next;
    }
}
