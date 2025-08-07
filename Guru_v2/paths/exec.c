/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 09:17:31 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 10:38:45 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/minishell.h"

int execute_2(char **inputs, t_cmd *data)
{
	int		i;
	char	**paths;
	int		index;

	i = 0;
	index = var_index("PATH=", data);
	paths = gen_paths(index, data, inputs[0]);
	while (paths[i])
	{
		if (access(paths[i], X_OK) == 0 &&
			execve(paths[i], inputs, data->env) != -1)
		{
			free_argv(paths,"exec1");
			return (0);
		}
		i++;
	}
	free_argv(paths,"exec2");
	return (1);
}

int execute(char **inputs, t_cmd *data)
{
	int	index;

	index = var_index("PATH=", data);
	if (ft_strchr(inputs[0], '/') &&
		access(inputs[0], X_OK) == 0 &&
		execve(inputs[0], &inputs[0], data->env) != -1)
	{
		return (0);
	}
	else if (index >= 0)
	{
		if (!execute_2(inputs, data))
			return (0);
	}
	return (1);
}
void	handle_exec(char **inputs, t_cmd *data)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (!check_exec(inputs, data))
		return (error_sentence("\t\tminishell: Unknown command\n", 127));
	pid = fork();
	if (pid == 0)
	{
		if (execute(inputs, data) != 0)
			exit(-1);
		exit(EXIT_SUCCESS);
	}
	else if (pid < 0)
		exit(EXIT_FAILURE);
	else
	{
		sig_exec_init();
		waitpid(pid, &status, 0);
	}
	g_status = WEXITSTATUS(status);
	if (g_quit)
		g_status = 130;
}
