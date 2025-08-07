/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/28 11:09:22 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 15:49:38 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_args(t_tokens *args)
{
	t_tokens	*current;
	const char	*type_str;

	current = args;
	while (current && current->token)
	{
		switch (current->type)
		{
		case TOKEN_CMD:
			type_str = "CMD";
			break ;
		case TOKEN_ARG:
			type_str = "ARG";
			break ;
		case TOKEN_PIPE:
			type_str = "PIPE";
			break ;
		case TOKEN_REDIRECT_IN:
			type_str = "REDIRECT_IN";
			break ;
		case TOKEN_REDIRECT_OUT:
			type_str = "REDIRECT_OUT";
			break ;
		case TOKEN_REDIRECT_APPEND:
			type_str = "REDIRECT_APPEND";
			break ;
		case TOKEN_HEREDOC:
			type_str = "HEREDOC";
			break ;
		case TOKEN_AND:
			type_str = "AND";
			break ;
		case TOKEN_OR:
			type_str = "OR";
			break ;
		case TOKEN_SEMICOLON:
			type_str = "SEMICOLON";
			break ;
		case TOKEN_BACKGROUND:
			type_str = "BACKGROUND";
			break ;
		case TOKEN_SQUOTED:
			type_str = "SQUOTED";
			break ;
		case TOKEN_DQUOTED:
			type_str = "DQUOTED";
			break ;
		case TOKEN_VARIABLE:
			type_str = "VARIABLE";
			break ;
		case TOKEN_ASSIGNMENT:
			type_str = "ASSIGNMENT";
			break ;
		case TOKEN_OPEN_PAREN:
			type_str = "OPEN_PAREN";
			break ;
		case TOKEN_CLOSE_PAREN:
			type_str = "CLOSE_PAREN";
			break ;
		default:
			type_str = "UNKNOWN";
			break ;
		}
		printf("%i : %s  {%s}\n", current->token_num, current->token, type_str);
		current = current->next;
	}
}




int main(int ac, char **av, char **envp)
{
    char        *inp_line;
    t_env       *env_list;
    int         status = 0;
    t_tokens    *args;

    (void)ac;
    (void)av;
    
    env_list = init_env(envp);
    init_signals();
    
    // Load command history from file
    // load_history_from_file();
    
    putstr_clr("Minishell v2.0 with Persistent History\n", 1, GREEN);
    printbanner();

    while (1)
    {
        inp_line = Readline();
        if (!inp_line) // Handle EOF (Ctrl+D)
        {
            printf("exit\n");
            // save_history_to_file();
            break;
        }
        
        if (ft_strlen(inp_line) == 0)
        {
            Free(inp_line, "empty_inp_line");
            continue;
        }

        // Add to history only if it's unique
        // add_to_history_if_unique(inp_line);

        args = input_parse2(inp_line);
        if (args)
        {
            status = execute_command_line(args, &env_list, envp);
            g_status = status;
            free_tokens(args, "main_tokens");
        }
        
        Free(inp_line, "main_inp_line");
    }
    
    // Cleanup
    if (env_list)
        free_env(env_list, "main_env");
    rl_clear_history();
    check_memory_leaks();
    
    return status;
}

