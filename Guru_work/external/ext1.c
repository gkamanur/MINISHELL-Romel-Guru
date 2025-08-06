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

    char **envp = malloc(sizeof(char *) * (count + 1));
    if (!envp)
        return NULL;

    current = env_list;
    int i = 0;
    while (current)
    {
        if (current->key && current->value)
        {
            envp[i] = malloc(strlen(current->key) + strlen(current->value) + 2);
            sprintf(envp[i], "%s=%s", current->key, current->value);
            i++;
        }
        current = current->next;
    }
    envp[i] = NULL;
    return envp;
}

// Helper to free envp array


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

    char **args = malloc(sizeof(char *) * (count + 1));
    if (!args)
        return NULL;

    current = tokens;
    int i = 0;
    while (current)
    {
        args[i] = current->token; // Assume token is already allocated
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
    char paths_copy[size_of_path];

    ft_strncpy(paths_copy, paths, sizeof(paths_copy));
    paths_copy[sizeof(paths_copy) - 1] = '\0';

    token = ft_strtok(paths_copy, ":");
    *count = 0;

    while (token) {
        result = Realloc(result, ((*count + 1) * sizeof(char*))," split_path_realloc");
        result[*count] = ft_strdup(token, "split_paths_strdup");
        if (!result[*count]) {
            perror("ft_strdup");
            return NULL;
        }

        (*count)++;
        token = ft_strtok(NULL, ":");
    }

    return result;
}

// Attempts to execute the command by searching paths and the current directory
int child_process(char** args, char** env) 
{
    char* path_string = get_path(env);
    int num_paths;
    char** path_list = split_paths(path_string, &num_paths);

    // access() execve()
    for (int i = 0; i < num_paths; i++) {
        char full_path[MAX_INPUT];
        snprintf(full_path, sizeof(full_path), "%s/%s", path_list[i], args[0]);

        if (access(full_path, X_OK) == 0) {
            if (execve(full_path, args, env) == -1)
            {
    	        perror("execve");
    	        free_argv(args,"free_args_child1");  // ← avoid leak
	        }
        }
    }

    for (int i = 0; path_list[i]; i++)
        free(path_list[i]);
    free(path_string);
    free(path_list);

    // Try executing the command in the current working directory
    char* cwd = NULL;
    cwd = getcwd(NULL, 0);
    if(cwd == NULL) {
        perror("getcwd");
        return 1;
    }

    char full_cwd_path[MAX_INPUT];
    snprintf(full_cwd_path, sizeof(full_cwd_path), "%s/%s", cwd, args[0]);
    //execve(full_cwd_path, args, env);
    if (execve(full_cwd_path, args, env) == -1) 
	{
    	perror("execve");
    	free_argv(args, "free_args_child2");  // ← avoid leak
	}	
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
        if (child_process(args, env)) {
            perror("execve");
            free_argv(args, "free_args_executor");
            return 1;
        }
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
        // if (WTERMSIG(status) == SIGSEGV) {
        //     perror("segmentation fault");
        // }
    }
    return 1;
}



// Fetches the PATH environment variable
