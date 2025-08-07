/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 15:39:34 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 15:39:48 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int has_redirection(t_tokens *tokens)
{
    while (tokens)
    {
        if (tokens->type == TOKEN_REDIRECT_IN || 
            tokens->type == TOKEN_REDIRECT_OUT ||
            tokens->type == TOKEN_REDIRECT_APPEND)
            return 1;
        tokens = tokens->next;
    }
    return 0;
}

int setup_input_redirection(char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        putstr_clr("minishell: ", 2, RED);
        perror(filename);
        return -1;
    }
    
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

int setup_output_redirection(char *filename, int append)
{
    int flags = O_WRONLY | O_CREAT;
    flags |= append ? O_APPEND : O_TRUNC;
    
    int fd = open(filename, flags, 0644);
    if (fd == -1)
    {
        putstr_clr("minishell: ", 2, RED);
        perror(filename);
        return -1;
    }
    
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return -1;
    }
    
    close(fd);
    return 0;
}

t_tokens *extract_command_tokens(t_tokens *tokens)
{
    t_tokens *head = NULL;
    t_tokens *current = tokens;
    int token_num = 0;
    
    while (current)
    {
        if (current->type == TOKEN_REDIRECT_IN ||
            current->type == TOKEN_REDIRECT_OUT ||
            current->type == TOKEN_REDIRECT_APPEND)
        {
            // Skip redirection operator and filename
            current = current->next;
            if (current)
                current = current->next;
        }
        else
        {
            add_token(&head, current->token, token_num++, current->type);
            current = current->next;
        }
    }
    
    return head;
}

int execute_with_redirection(t_tokens *tokens, t_env **env_list, char **envp)
{
    int stdin_backup = dup(STDIN_FILENO);
    int stdout_backup = dup(STDOUT_FILENO);
    t_tokens *current = tokens;
    int status = 0;
    
    // Process redirections
    while (current)
    {
        if (current->type == TOKEN_REDIRECT_IN)
        {
            if (!current->next || !current->next->token)
            {
                putstr_clr("minishell: syntax error near unexpected token `newline'\n", 2, RED);
                status = 2;
                goto cleanup;
            }
            
            if (setup_input_redirection(current->next->token) == -1)
            {
                status = 1;
                goto cleanup;
            }
        }
        else if (current->type == TOKEN_REDIRECT_OUT)
        {
            if (!current->next || !current->next->token)
            {
                putstr_clr("minishell: syntax error near unexpected token `newline'\n", 2, RED);
                status = 2;
                goto cleanup;
            }
            
            if (setup_output_redirection(current->next->token, 0) == -1)
            {
                status = 1;
                goto cleanup;
            }
        }
        else if (current->type == TOKEN_REDIRECT_APPEND)
        {
            if (!current->next || !current->next->token)
            {
                putstr_clr("minishell: syntax error near unexpected token `newline'\n", 2, RED);
                status = 2;
                goto cleanup;
            }
            
            if (setup_output_redirection(current->next->token, 1) == -1)
            {
                status = 1;
                goto cleanup;
            }
        }
        
        current = current->next;
    }
    
    // Extract command tokens (without redirection operators)
    t_tokens *cmd_tokens = extract_command_tokens(tokens);
    if (cmd_tokens)
    {
        status = execute_command_line(cmd_tokens, env_list, envp);
        free_tokens(cmd_tokens, "redirection_cmd");
    }
    
cleanup:
    // Restore original file descriptors
    dup2(stdin_backup, STDIN_FILENO);
    dup2(stdout_backup, STDOUT_FILENO);
    close(stdin_backup);
    close(stdout_backup);
    
    return status;
}
