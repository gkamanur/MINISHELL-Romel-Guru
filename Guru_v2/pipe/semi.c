/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 09:51:10 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:27:48 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h" 

int handle_semicolon(t_tokens *tokens, t_env **env_list, char **envp)
{
    t_tokens *current = tokens;
    t_tokens *cmd_start = tokens;
    int last_status = 0;
    
    while (current)
    {
        if (current->type == TOKEN_SEMICOLON || current->next == NULL)
        {
            // Execute command from cmd_start to current
            t_tokens *cmd = NULL;
            t_tokens *temp = cmd_start;
            
            while (temp != current && temp)
            {
                add_token(&cmd, temp->token, temp->token_num, temp->type);
                temp = temp->next;
            }
            
            if (cmd)
            {
                if (is_builtin(&cmd))
                    last_status = execute_builtin(cmd, env_list, last_status, NULL);
                else
                {
                    char **args = tokens_to_array(cmd);
                    if (args)
                    {
                        last_status = executor(args, envp);
                        free_argv(args, "semi_args");
                    }
                }
                free_tokens(cmd, "semi_cmd");
            }
            
            cmd_start = current->next;
        }
        current = current->next;
    }
    
    return last_status;
}

int handle_logical_and(t_tokens *tokens, t_env **env_list, char **envp)
{
    t_tokens *current = tokens;
    t_tokens *cmd_start = tokens;
    int last_status = 0;
    
    while (current)
    {
        if (current->type == TOKEN_AND || current->next == NULL)
        {
            t_tokens *cmd = NULL;
            t_tokens *temp = cmd_start;
            
            while (temp != current && temp)
            {
                add_token(&cmd, temp->token, temp->token_num, temp->type);
                temp = temp->next;
            }
            
            if (cmd)
            {
                if (is_builtin(&cmd))
                    last_status = execute_builtin(cmd, env_list, last_status, NULL);
                else
                {
                    char **args = tokens_to_array(cmd);
                    if (args)
                    {
                        last_status = executor(args, envp);
                        free_argv(args, "and_args");
                    }
                }
                free_tokens(cmd, "and_cmd");
                
                // If command failed, don't execute next
                if (last_status != 0)
                    break;
            }
            
            cmd_start = current->next;
        }
        current = current->next;
    }
    
    return last_status;
}

int handle_logical_or(t_tokens *tokens, t_env **env_list, char **envp)
{
    t_tokens *current = tokens;
    t_tokens *cmd_start = tokens;
    int last_status = 0;
    
    while (current)
    {
        if (current->type == TOKEN_OR || current->next == NULL)
        {
            t_tokens *cmd = NULL;
            t_tokens *temp = cmd_start;
            
            while (temp != current && temp)
            {
                add_token(&cmd, temp->token, temp->token_num, temp->type);
                temp = temp->next;
            }
            
            if (cmd)
            {
                if (is_builtin(&cmd))
                    last_status = execute_builtin(cmd, env_list, last_status, NULL);
                else
                {
                    char **args = tokens_to_array(cmd);
                    if (args)
                    {
                        last_status = executor(args, envp);
                        free_argv(args, "or_args");
                    }
                }
                free_tokens(cmd, "or_cmd");
                
                // If command succeeded, don't execute next
                if (last_status == 0)
                    break;
            }
            
            cmd_start = current->next;
        }
        current = current->next;
    }
    
    return last_status;
}
