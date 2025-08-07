/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:32:24 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int builtin_cd(t_tokens *tokens, t_env **env_list)
{
    char *path = NULL;
    char *old_pwd;
    char *new_pwd;
    
    if (!tokens->next)
    {
        // No argument, go to HOME
        path = get_env_value("HOME", *env_list);
        if (!path)
        {
            putstr_fd("cd: HOME not set\n", 2);
            return 1;
        }
    }
    else if (tokens->next->next)
    {
        putstr_fd("cd: too many arguments\n", 2);
        return 1;
    }
    else
    {
        path = tokens->next->token;
        if (ft_strcmp(path, "-") == 0)
        {
            path = get_env_value("OLDPWD", *env_list);
            if (!path)
            {
                putstr_fd("cd: OLDPWD not set\n", 2);
                return 1;
            }
            printf("%s\n", path);
        }
    }
    
    old_pwd = getcwd(NULL, 0);
    if (!old_pwd)
    {
        perror("cd: getcwd");
        return 1;
    }
    
    if (chdir(path) != 0)
    {
        perror("cd");
        free(old_pwd);
        return 1;
    }
    
    new_pwd = getcwd(NULL, 0);
    if (!new_pwd)
    {
        perror("cd: getcwd");
        free(old_pwd);
        return 1;
    }
    
    set_env_variable("OLDPWD", old_pwd, env_list);
    set_env_variable("PWD", new_pwd, env_list);
    
    free(old_pwd);
    free(new_pwd);
    
    return 0;
}
// int builtin_cd(t_tokens *tokens, t_env **env_list)
// {
//     char    **args;
//     char    *path;
//     int     status;

//     args = tokens_to_args(tokens); // Malloc happens here
//     if (!args)
//         return (1);
//     path = args[1] ? args[1] : get_env_value("HOME", *env_list);
//     status = 0;
//     if (chdir(path) != 0)
//     {
//         perror("cd");
//         status = 1;
//     }
//     free(args); // The args array is freed here
//     // The code then continues, and this is where the use-after-free likely happens
//     return (status);
// }
