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

char	**env_to_array(t_env *env_list)
{
    t_env	*current;
    char	**env_array;
    char	*temp;
    int		count;
    int		i;

    // Count the number of environment variables
    count = 0;
    current = env_list;
    while (current)
    {
        count++;
        current = current->next;
    }

    // Allocate memory for the array (+1 for NULL terminator)
    env_array = (char **)malloc(sizeof(char *) * (count + 1));
    if (!env_array)
        return (NULL);

    // Convert each env node to "key=value" string
    current = env_list;
    i = 0;
    while (current)
    {
        // Allocate memory for key=value string
        temp = (char *)malloc(sizeof(char) * (strlen(current->key) + strlen(current->value) + 2));
        if (!temp)
        {
            // Free previously allocated strings and array on failure
            while (i > 0)
                free(env_array[--i]);
            free(env_array);
            return (NULL);
        }
        // Concatenate key, "=", and value
        strcpy(temp, current->key);
        strcat(temp, "=");
        strcat(temp, current->value);
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
    
    pid_t pid = Fork();
    if (pid == 0)
    {
        // Child process
        if (execve(args[0], args, env_array) == -1)
        {
            // Try to find command in PATH
            char *path_env = get_env_value("PATH", *env_list);
            if (path_env)
            {
                char **paths = ft_split(path_env, ':');
                for (int i = 0; paths[i]; i++)
                {
                    char *full_path = Malloc(ft_strlen(paths[i]) + ft_strlen(args[0]) + 2, "full_path");
                    ft_strncpy(full_path, paths[i], ft_strlen(paths[i]));
                    ft_strcat(full_path, "/");
                    ft_strcat(full_path, args[0]);
                    
                    if (execve(full_path, args, env_array) != -1)
                        break;
                    Free(full_path, "full_path");
                }
                free_argv(paths, "paths");
            }
            
            putstr_clr("minishell: command not found: ", 2, RED);
            putstr_fd(args[0], 2);
            putstr_fd("\n", 2);
            exit(127);
        }
    }
    else
    {
        // Parent process
        int status;
        Waitpid(pid, &status, 0);
        free_argv(args, "args");
        free_argv(env_array, "env_array");
        return WEXITSTATUS(status);
    }
    
    return 0;
}

// int execute_command_line(t_tokens *tokens, t_env **env_list, char **envp)
// {
//     if (!tokens)
//         return 0;
        
//     // Handle different types of command combinations
//     if (has_pipe(tokens))
//         return handle_pipe(tokens, env_list, envp);
//     else if (has_logical_and(tokens))
//         return handle_logical_and(tokens, env_list, envp);
//     else if (has_logical_or(tokens))
//         return handle_logical_or(tokens, env_list, envp);
//     else if (has_semicolon(tokens))
//         return handle_semicolon(tokens, env_list, envp);
//     else if (has_redirection(tokens))
//         return execute_with_redirection(tokens, env_list, envp);
//     else
//     {
//         // Simple command
//         if (is_builtin(&tokens))
//             return execute_builtin(tokens, env_list, 0, NULL);
//         else
//         {
//             char **args = tokens_to_array(tokens);
//             if (args)
//             {
//                 int status = executor(args, envp);
//                 free_argv(args, "simple_cmd_args");
//                 return status;
//             }
//         }
//     }
    
//     return 0;
// }
