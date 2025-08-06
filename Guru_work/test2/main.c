#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>



// Include your shell headers here
// #include "minishell.h"
typedef struct s_data {
	char **env; // array of strings like "PATH=/usr/bin:/bin"
} t_data;

// strdup alternative
char *ft_strdup(const char *s)
{
	char *dup = malloc(strlen(s) + 1);
	if (!dup)
		return NULL;
	strcpy(dup, s);
	return dup;
}
void print_array(char **array)
{
    int i = 0;
    if (!array)
    {
        printf("(null)\n");
        return;
    }
    while (array[i])
    {
        printf("[%d]: %s\n", i, array[i]);
        i++;
    }
}

// strchr alternative
char *ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return (char *)s;
		s++;
	}
	return NULL;
}

// strjoin: concatenates two strings
char *ft_strjoin(const char *s1, const char *s2)
{
	size_t len1 = strlen(s1);
	size_t len2 = strlen(s2);
	char *joined = malloc(len1 + len2 + 1);
	if (!joined)
		return NULL;
	strcpy(joined, s1);
	strcat(joined, s2);
	return joined;
}

// Split string on a delimiter (e.g. ':' for PATH)
char **ft_split(const char *str, char delimiter)
{
	int count = 1;
	for (int i = 0; str[i]; i++)
		if (str[i] == delimiter)
			count++;

	char **result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return NULL;
	int start = 0, idx = 0;
	for (int i = 0; ; i++) {
		if (str[i] == delimiter || str[i] == '\0') {
			int len = i - start;
			result[idx] = malloc(len + 1);
			if (!result[idx])
				return NULL; // leak-prone in error case
			strncpy(result[idx], &str[start], len);
			result[idx][len] = '\0';
			idx++;
			start = i + 1;
			if (str[i] == '\0')
				break;
		}
	}
	result[idx] = NULL;
	return result;
}

// Free NULL-terminated array of strings
void free_env(char **env)
{
	if (!env)
		return;
	for (int i = 0; env[i]; i++)
		free(env[i]);
	free(env);
}

// Return index of an env variable like "PATH="
int var_index(const char *prefix, struct s_data *data)
{
	int i = 0;
	size_t len = strlen(prefix);

	while (data->env[i])
	{
		if (strncmp(data->env[i], prefix, len) == 0)
			return i;
		i++;
	}
	return -1;
}

char **gen_paths(char *path_env, char *input)
{
	char	**dirs;
	char	*full_path;
	int		i = 0;
    printf("realpath::::: %s\n", path_env);
    dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
    print_array(dirs);
	while (dirs[i])
	{
		full_path = ft_strjoin(dirs[i], "/");
		free(dirs[i]);
		dirs[i] = ft_strjoin(full_path, input);
		free(full_path);
		i++;
	}
    printf("after\n");
    print_array(dirs);
	return (dirs);
}
int	check_exec_path(char **inputs, t_data *data)
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



int	main(int argc, char **argv, char **envp)
{
	t_data	data;
	char	*input_cmd[] = {"grep", NULL}; // Change "ls" to any binary name to test

	// Assign environment
	data.env = envp;

	// Test the execution checker
	if (check_exec(input_cmd, &data))
		printf("✅ Command '%s' found and executable.\n", input_cmd[0]);
	else
		printf("❌ Command '%s' not found or not executable.\n", input_cmd[0]);

	return (0);
}
