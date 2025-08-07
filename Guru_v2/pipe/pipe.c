/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 09:51:10 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:27:33 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h" 

int handle_pipe(t_tokens *tokens, t_env **env_list, char **envp)
{
    int pipefd[2];
    pid_t pid1, pid2;
    int status;
    t_tokens *left_cmd = NULL;
    t_tokens *right_cmd = NULL;
    t_tokens *current = tokens;
    
    // Find the pipe and split commands
    while (current && current->type != TOKEN_PIPE)
    {
        add_token(&left_cmd, current->token, current->token_num, current->type);
        current = current->next;
    }
    
    if (!current || current->type != TOKEN_PIPE)
        return 1;
        
    current = current->next; // Skip pipe token
    
    while (current)
    {
        add_token(&right_cmd, current->token, current->token_num, current->type);
        current = current->next;
    }
    
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }
    
    pid1 = fork();
    if (pid1 == 0)
    {
        // Left command - write to pipe
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        
        if (is_builtin(&left_cmd))
            exit(execute_builtin(left_cmd, env_list, 0, NULL));
        else
        {
            char **args = tokens_to_array(left_cmd);
            execve(args[0], args, envp);
            perror("execve");
            exit(1);
        }
    }
    
    pid2 = fork();
    if (pid2 == 0)
    {
        // Right command - read from pipe
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        
        if (is_builtin(&right_cmd))
            exit(execute_builtin(right_cmd, env_list, 0, NULL));
        else
        {
            char **args = tokens_to_array(right_cmd);
            execve(args[0], args, envp);
            perror("execve");
            exit(1);
        }
    }
    
    close(pipefd[0]);
    close(pipefd[1]);
    
    waitpid(pid1, &status, 0);
    waitpid(pid2, &status, 0);
    
    free_tokens(left_cmd, "pipe_left");
    free_tokens(right_cmd, "pipe_right");
    
    return WEXITSTATUS(status);
}
