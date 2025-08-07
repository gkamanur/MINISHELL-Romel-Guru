/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ext1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 15:53:14 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/05 10:21:45 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Helper to convert env_list to char ** for execve
char **env_list_to_array(t_env *env_list)
{
    int count = 0;
    t_env *current = env_list;
    while (current)
    {
        if (current->key && current->value)
            count++;
        current = current->next;
    }

    char **envp = Malloc(sizeof(char *) * (count + 1), "env_array");
    if (!envp)
        return NULL;

    current = env_list;
    int i = 0;
    while (current)
    {
        if (current->key && current->value)
        {
            size_t key_len = ft_strlen(current->key);
            size_t val_len = ft_strlen(current->value);
            envp[i] = Malloc(key_len + val_len + 2, "env_entry");
            sprintf(envp[i], "%s=%s", current->key, current->value);
            i++;
        }
        current = current->next;
    }
    envp[i] = NULL;
    return envp;
}

// Helper to convert tokens to char ** for execve
char **tokens_to_array(t_tokens *tokens)
{
    int count = 0;
    t_tokens *current = tokens;
    while (current)
    {
        count++;
        current = current->next;
    }

    char **args = Malloc(sizeof(char *) * (count + 1), "args_array");
    if (!args)
        return NULL;

    current = tokens;
    int i = 0;
    while (current)
    {
        args[i] = ft_strdup(current->token, "args_token"); // Create copy instead of direct reference
        i++;
        current = current->next;
    }
    args[i] = NULL;
    return args;
}

char* get_path(char** env) {
    for (int i = 0; env[i]; i++) {
        if(ft_strncmp(env[i], "PATH=", 5) == 0) {
            return ft_strdup(env[i] + 5, "get_path_strdup");
        }
    }
    return NULL;
}

// Split the PATH string into individual paths
char** split_paths(char* paths, int* count) {
    char** result = NULL;
    char* token;
    size_t size_of_path = ft_strlen(paths);
    char *paths_copy = Malloc(size_of_path + 1, "paths_copy");

    ft_strncpy(paths_copy, paths, size_of_path);
    paths_copy[size_of_path] = '\0';

    token = ft_strtok(paths_copy, ":");
    *count = 0;

    while (token) {
        result = Realloc(result, ((*count + 1) * sizeof(char*))," split_path_realloc");
        result[*count] = ft_strdup(token, "split_paths_strdup");
        if (!result[*count]) {
            perror("ft_strdup");
            // Clean up on failure
            for (int j = 0; j < *count; j++)
                Free(result[j], "cleanup_split_paths");
            Free(result, "cleanup_split_paths");
            Free(paths_copy, "cleanup_paths_copy");
            return NULL;
        }

        (*count)++;
        token = ft_strtok(NULL, ":");
    }

    Free(paths_copy, "paths_copy");
    return result;
}

// Attempts to execute the command by searching paths and the current directory
int child_process(char** args, char** env) 
{
    char* path_string = get_path(env);
    if (!path_string)
    {
        // Try executing in current directory
        if (execve(args[0], args, env) == -1)
        {
            perror("execve");
            free_argv(args, "free_args_child_no_path");
            exit(127);
        }
    }
    
    int num_paths;
    char** path_list = split_paths(path_string, &num_paths);
    
    if (!path_list)
    {
        Free(path_string, "path_string");
        if (execve(args[0], args, env) == -1)
        {
            perror("execve");
            free_argv(args, "free_args_child_no_paths");
            exit(127);
        }
    }

    // Try each path
    for (int i = 0; i < num_paths; i++) {
        char *full_path = Malloc(ft_strlen(path_list[i]) + ft_strlen(args[0]) + 2, "full_path");
        sprintf(full_path, "%s/%s", path_list[i], args[0]);

        if (access(full_path, X_OK) == 0) {
            if (execve(full_path, args, env) == -1)
            {
                perror("execve");
                Free(full_path, "full_path");
                // Clean up before exit
                for (int j = 0; j < num_paths; j++)
                    Free(path_list[j], "path_cleanup");
                Free(path_list, "path_list_cleanup");
                Free(path_string, "path_string_cleanup");
                free_argv(args, "free_args_child1");
                exit(127);
            }
        }
        Free(full_path, "full_path");
    }

    // Clean up path resources
    for (int i = 0; i < num_paths; i++)
        Free(path_list[i], "path_cleanup");
    Free(path_list, "path_list_cleanup");
    Free(path_string, "path_string_cleanup");

    // Try executing the command in the current working directory
    char* cwd = getcwd(NULL, 0);
    if(cwd == NULL) {
        perror("getcwd");
        free_argv(args, "free_args_child_getcwd_fail");
        exit(1);
    }

    char *full_cwd_path = Malloc(ft_strlen(cwd) + ft_strlen(args[0]) + 2, "full_cwd_path");
    sprintf(full_cwd_path, "%s/%s", cwd, args[0]);
    
    if (execve(full_cwd_path, args, env) == -1) 
    {
        perror("execve");
        Free(full_cwd_path, "full_cwd_path");
        free(cwd);
        free_argv(args, "free_args_child2");
        exit(127);
    }
    
    // This should never be reached
    Free(full_cwd_path, "full_cwd_path");
    free(cwd);
    return 1;
}

int executor(char** args, char** env)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) // child process
    { 
        child_process(args, env);
        // Should not reach here
        exit(127);
    } 
    else // Parent process
    { 
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid");
            return 1;
        }
        if (WIFSIGNALED(status)) {
            printf("Process terminated by signal: %d\n", WTERMSIG(status));
        }
        return WEXITSTATUS(status);
    }
}
