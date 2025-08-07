/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_main.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 10:08:58 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 15:59:53 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h" 

int has_pipe(t_tokens *tokens)
{
    while (tokens)
    {
        if (tokens->type == TOKEN_PIPE)
            return 1;
        tokens = tokens->next;
    }
    return 0;
}

int has_semicolon(t_tokens *tokens)
{
    while (tokens)
    {
        if (tokens->type == TOKEN_SEMICOLON)
            return 1;
        tokens = tokens->next;
    }
    return 0;
}

int has_logical_and(t_tokens *tokens)
{
    while (tokens)
    {
        if (tokens->type == TOKEN_AND)
            return 1;
        tokens = tokens->next;
    }
    return 0;
}

int has_logical_or(t_tokens *tokens)
{
    while (tokens)
    {
        if (tokens->type == TOKEN_OR)
            return 1;
        tokens = tokens->next;
    }
    return 0;
}

char **env_to_array(t_env *env_list)
{
    t_env *current;
    char **env_array;
    char *temp;
    int count;
    int i;

    // Count the number of environment variables
    count = 0;
    current = env_list;
    while (current)
    {
        count++;
        current = current->next;
    }

    // Allocate memory for the array (+1 for NULL terminator)
    env_array = Malloc(sizeof(char *) * (count + 1), "env_array");
    if (!env_array)
        return (NULL);

    // Convert each env node to "key=value" string
    current = env_list;
    i = 0;
    while (current)
    {
        // Allocate memory for key=value string
        size_t key_len = ft_strlen(current->key);
        size_t val_len = ft_strlen(current->value);
        temp = Malloc(key_len + val_len + 2, "env_string");
        if (!temp)
        {
            // Free previously allocated strings and array on failure
            while (i > 0)
                Free(env_array[--i], "env_cleanup");
            Free(env_array, "env_array_cleanup");
            return (NULL);
        }
        // Concatenate key, "=", and value
        ft_strncpy(temp, current->key, key_len);
        temp[key_len] = '=';
        ft_strncpy(temp + key_len + 1, current->value, val_len);
        temp[key_len + val_len + 1] = '\0';
        
        env_array[i] = temp;
        i++;
        current = current->next;
    }
    env_array[i] = NULL; // Null-terminate the array
    return (env_array);
}

int execute_command_line(t_tokens *tokens, t_env **env_list, char **envp)
{
    if (!tokens)
        return 0;

    // Handle different command structures
    if (has_pipe(tokens))
        return handle_pipe(tokens, env_list, envp);
    else if (has_semicolon(tokens))
        return handle_semicolon(tokens, env_list, envp);
    else if (has_logical_and(tokens))
        return handle_logical_and(tokens, env_list, envp);
    else if (has_logical_or(tokens))
        return handle_logical_or(tokens, env_list, envp);
    else if (has_redirection(tokens))
        return execute_with_redirection(tokens, env_list, envp);
    
    // Simple command execution
    if (is_builtin(&tokens))
        return execute_builtin(tokens, env_list, g_status, NULL);
    
    // External command
    char **args = tokens_to_array(tokens);
    char **env_array = env_to_array(*env_list);
    int status = 0;
    
    if (!args || !env_array)
    {
        if (args) free_argv(args, "args_cleanup");
        if (env_array) free_argv(env_array, "env_array_cleanup");
        return 1;
    }
    
    pid_t pid = Fork();
    if (pid == 0)
    {
        // Child process
        if (execve(args[0], args, env_array) == -1)
        {
            // Try to find command in PATH
            char *path_env = get_env_value("PATH", *env_list);
            if (path_env && ft_strlen(path_env) > 0)
            {
                char **paths = ft_split(path_env, ':');
                if (paths)
                {
                    for (int i = 0; paths[i]; i++)
                    {
                        char *full_path = Malloc(ft_strlen(paths[i]) + ft_strlen(args[0]) + 2, "full_path");
                        ft_strncpy(full_path, paths[i], ft_strlen(paths[i]));
                        full_path[ft_strlen(paths[i])] = '/';
                        ft_strncpy(full_path + ft_strlen(paths[i]) + 1, args[0], ft_strlen(args[0]));
                        full_path[ft_strlen(paths[i]) + ft_strlen(args[0]) + 1] = '\0';
                        
                        if (execve(full_path, args, env_array) != -1)
                            break;
                        Free(full_path, "full_path");
                    }
                    free_argv(paths, "paths");
                }
            }
            
            putstr_clr("minishell: command not found: ", 2, RED);
            putstr_fd(args[0], 2);
            putstr_fd("\n", 2);
            
            // Clean up in child before exit
            free_argv(args, "args_child");
            free_argv(env_array, "env_array_child");
            exit(127);
        }
    }
    else
    {
        // Parent process
        Waitpid(pid, &status, 0);
        free_argv(args, "args");
        free_argv(env_array, "env_array");
        return WEXITSTATUS(status);
    }
    
    return 0;
}
