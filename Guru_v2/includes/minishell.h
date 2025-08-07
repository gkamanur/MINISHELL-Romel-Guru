/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:55:14 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 15:52:14 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ps -eo pid,comm | grep '[m]inishell' | awk '{print $1}' | xargs kill     */
#ifndef MINISHELL_H
# define MINISHELL_H
# define _DEFAULT_SOURCE
# include "colors.h"
# include <stdio.h>
# include <stdarg.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <string.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <sysexits.h>
# include <signal.h>
# include <fcntl.h>
#define SPAACE "\a\t\n\v\f\r "
# define BUFSIZEE 1024
# define MAX_INPUT 4096

typedef enum {
    TOKEN_CMD,        // Command (e.g., ls, echo)
    TOKEN_ARG,        // Command argument (e.g., -l, filename)
    TOKEN_PIPE,       // Pipe operator (|)
    TOKEN_REDIRECT_IN,     // Input redirection (<)
    TOKEN_REDIRECT_OUT,    // Output redirection (>)
    TOKEN_REDIRECT_APPEND, // Append redirection (>>)
    TOKEN_HEREDOC,         // Heredoc (<<)
    TOKEN_AND,             // Logical AND (&&)
    TOKEN_OR,              // Logical OR (||)
    TOKEN_SEMICOLON,       // Command separator (;)
    TOKEN_BACKGROUND,      // Background process (&)
    TOKEN_SQUOTED,         // Single-quoted string ('...')
    TOKEN_DQUOTED,         // Double-quoted string ("...")
    TOKEN_VARIABLE,        // Environment variable reference ($HOME)
    TOKEN_ASSIGNMENT,      // Variable assignment (e.g., VAR=value)
    TOKEN_OPEN_PAREN,      // Open parenthesis ( for subshells or grouping
    TOKEN_CLOSE_PAREN,     // Close parenthesis )
    TOKEN_UNKNOWN          // Unrecognized token
} t_token_type;

typedef struct s_tokens 
{
    char *token;
    int token_num;
    t_token_type type;
    struct s_tokens *next;
} t_tokens;

typedef struct s_parse
{
	t_tokens	**head;
	char		*buffer;
	int			token_num;
	size_t		i;
}				t_parse;

typedef struct s_cmd {
    char **env;
    char *input_file;
    char *output_file;
    char *pwd;
    int redir;
} t_cmd;

typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_builtin
{
    const char *builtin_name;
	int (*fun)(char **av);
} t_builtin;

/* Global variables */
extern int g_status;
extern char *g_user_input;
extern int g_quit;

/* Debug */
void log_debug(const char *fmt, ...);

/* Utility functions */
void	putstr_fd(char *str, int fd);
void	putstr_clr(char *str, int fd, char *clr);
int	is_operator(const char *str);
int	is_special_char(int c);
int	lst_size(t_tokens *lst);
pid_t	Fork(void);
pid_t	Waitpid(pid_t pid, int *status, int options);
void printbanner(void);
void dbzSpinnerLoading(void);
void exitLoading(void);
char *Readline(void);
void init_signals(void);
void handle_sigint(int sig);
void handle_sigquit(int sig);

/* Memory management */
void *Malloc (size_t size, const char *name);
void *Realloc (void *ptr, size_t size, const char *name);
char	*ft_strdup(const char *src, const char *name);
void free_argv(char **argv, const char *name);
void free_envp(char **envp, const char *name);
void free_env(t_env *env_list, const char *name);
void free_tokens(t_tokens *head, const char *name);
void Free(void *ptr, const char *name);
void check_memory_leaks(void);

/* String functions */
char* ft_strncpy(char* dest, const char* src, size_t n);
size_t	ft_strlen(const char *str);
int	ft_strcmp(const char *s1, const char *s2);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char* ft_strtok(char* input_string, const char* delimiter);
int ft_strncmp(const char* str1, const char* str2, size_t n);
int	ft_isalpha(int c);
char	*ft_strchr(const char *str, int search_str);
char	**ft_split(const char *s, char c);
int ft_isalnum(int c);
char *ft_substr(char const *s, unsigned int start, size_t len);
char *ft_itoa(int n);
char *ft_strjoin(char const *s1, char const *s2);
char	*ft_strcat(char *dest, const char *src);

/* Parsing functions */
int	ft_check(char c);
void add_token(t_tokens **head, char *token, int token_num, t_token_type type);
void skip_whitespace(char *input, size_t len, t_parse *ctx);
t_tokens	*input_parse2(char *input);
int handle_quote(char *input, size_t len, t_parse *ctx);

/* Environment functions */
t_env	*create_env_node(const char *key, const char *value);
t_env	*init_env(char **envp);
char	*get_env_value(const char *key, t_env *env_list);
void set_env_variable(const char *key, const char *value, t_env **env_list);
void unset_env_variable(const char *key, t_env **env_list);
char *expand_variable(const char *var_name, t_env *env_list);
char *process_quotes_and_vars(const char *input, t_env *env_list);

/* Builtin commands */
int builtin_cd(t_tokens *tokens, t_env **env_list);
int builtin_echo(t_tokens *tokens, t_env **env_list);
int builtin_env(t_tokens *tokens, t_env **env_list);
int builtin_export(t_tokens *tokens, t_env **env_list);
int builtin_pwd(t_tokens *tokens, t_env **env_list);
int builtin_unset(t_tokens *tokens, t_env **env_list);
void builtin_exit(t_tokens *tokens, t_env **env_list, char *inp_line);

/* Execution functions */
char **tokens_to_args(t_tokens *tokens_head);
char **tokens_to_array(t_tokens *tokens);
int is_builtin(t_tokens **tokens);
int execute_builtin(t_tokens *tokens, t_env **env_list, int last_status, char *inp_line);
int executor(char** args, char** env);
int execute_command_line(t_tokens *tokens, t_env **env_list, char **envp);

/* Path and execution helpers */
int var_index(char *var, t_cmd *data);
char **gen_paths(int index, t_cmd *data, char *cmd);
int check_exec(char **inputs, t_cmd *data);
void error_sentence(char *msg, int code);
void sig_exec_init(void);
void handle_exec(char **inputs, t_cmd *data);

/* Command handling */
int handle_pipe(t_tokens *tokens, t_env **env_list, char **envp);
int handle_semicolon(t_tokens *tokens, t_env **env_list, char **envp);
int handle_logical_and(t_tokens *tokens, t_env **env_list, char **envp);
int handle_logical_or(t_tokens *tokens, t_env **env_list, char **envp);
int has_pipe(t_tokens *tokens);
int has_semicolon(t_tokens *tokens);
int has_logical_and(t_tokens *tokens);
int has_logical_or(t_tokens *tokens);

int has_redirection(t_tokens *tokens);
int execute_with_redirection(t_tokens *tokens, t_env **env_list, char **envp);

#endif
/*#echo "Hello $echo "Hello $USER" | grep Hello > out.txt; cat < out.txt && export VAR="42"*/
