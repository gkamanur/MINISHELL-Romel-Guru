/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_paths.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:47:02 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/06 13:53:11 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char **gen_paths(char *path_env, char *input)
{
	char	**dirs;
	char	*full_path;
	int		i = 0;
    // printf("realpath::::: %s\n", path_env);
    dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
    // print_array(dirs);
	while (dirs[i])
	{
		full_path = ft_strjoin(dirs[i], "/");
		free(dirs[i]);
		dirs[i] = ft_strjoin(full_path, input);
		free(full_path);
		i++;
	}
    // printf("after\n");
    // print_array(dirs);
	return (dirs);
}
int	check_exec_path(char **inputs, t_cmd *data)
{
	int			i = 0;
	int			path_i = var_index("PATH=", data);
	char		**paths;
	struct stat	st;

	if (path_i == -1 || !inputs[0])
		return (0);

	char *path_value = data->env[path_i] + 5; // Skip "PATH="
	paths = gen_paths(path_value, inputs[0]);

	while (paths && paths[i])
	{
		if (stat(paths[i], &st) == 0 &&
			(st.st_mode & S_IXUSR) &&
			!(st.st_mode & __S_IFDIR))
		{
			free_env(paths);
			return (1);
		}
		i++;
	}
	free_env(paths);
	return (0);
}
int	check_exec(char **inputs, t_data *data)
{
	struct stat	st;

	if (!inputs || !inputs[0])
		return (0);

	if (ft_strchr(inputs[0], '/'))
	{
		if (stat(inputs[0], &st) == 0 &&
			(st.st_mode & S_IXUSR) &&
			!(st.st_mode & __S_IFDIR))
			return (1);
	}
	return check_exec_path(inputs, data);
}