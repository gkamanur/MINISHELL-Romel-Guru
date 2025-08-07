/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:47:02 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:36:46 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int var_index(char *var, t_cmd *data)
{
    int i = 0;
    
    while (data->env[i])
    {
        if (ft_strncmp(data->env[i], var, ft_strlen(var)) == 0)
            return i;
        i++;
    }
    return -1;
}

char **gen_paths(int index, t_cmd *data, char *cmd)
{
    char *path_env;
    char **paths;
    char **full_paths;
    int i = 0;
    int path_count = 0;
    
    if (index < 0)
        return NULL;
        
    path_env = data->env[index] + 5; // Skip "PATH="
    paths = ft_split(path_env, ':');
    
    // Count paths
    while (paths[path_count])
        path_count++;
        
    full_paths = Malloc(sizeof(char *) * (path_count + 1), "gen_paths");
    
    while (paths[i])
    {
        full_paths[i] = Malloc(ft_strlen(paths[i]) + ft_strlen(cmd) + 2, "gen_paths_full");
        sprintf(full_paths[i], "%s/%s", paths[i], cmd);
        i++;
    }
    full_paths[i] = NULL;
    
    free_argv(paths, "gen_paths_cleanup");
    return full_paths;
}

int check_exec(char **inputs, t_cmd *data)
{
    if (!inputs || !inputs[0])
        return 0;
        
    // Check if it's an absolute/relative path
    if (ft_strchr(inputs[0], '/'))
        return (access(inputs[0], X_OK) == 0);
        
    // Check in PATH
    int index = var_index("PATH=", data);
    if (index >= 0)
    {
        char **paths = gen_paths(index, data, inputs[0]);
        if (!paths)
            return 0;
            
        int i = 0;
        while (paths[i])
        {
            if (access(paths[i], X_OK) == 0)
            {
                free_argv(paths, "check_exec");
                return 1;
            }
            i++;
        }
        free_argv(paths, "check_exec");
    }
    
    return 0;
}

void error_sentence(char *msg, int code)
{
    putstr_fd(msg, 2);
    g_status = code;
}

void sig_exec_init(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}
