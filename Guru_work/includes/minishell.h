/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 10:55:14 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/06 13:51:47 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ps -eo pid,comm | grep '[m]inishell' | awk '{print $1}' | xargs kill     */
#ifndef MINISHELL_H
# define MINISHELL_H
# define _DEFAULT_SOURCE
# include "colors.h"
# include <limits.h>
# include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sysexits.h>
# include <signal.h>
#define SPAACE "\a\t\n\v\f\r "
# define BUFSIZEE 1024

// typedef enum {
//     TOKEN_WORD,      // Commands, arguments, filenames 
//     TOKEN_SQUOTED,   //single Quoted strings
//     TOKEN_DQUOTED,   //double Quoted strings
//     TOKEN_OPERATOR,  // Operators like |, >, <, >>, &&
//     TOKEN_VARIABLE   // Variables like $HOME
// } t_token_type;

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
    char **args;
    char *input_file;
    char *output_file;
    int append;
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

/* Debugg*/
void log_debug(const char *fmt, ...);

void	putstr_fd(char *str, int fd);
void	putstr_clr(char *str, int fd, char *clr);
int	is_operator(const char *str);
int	is_special_char(int c);
int	lst_size(t_tokens *lst);
pid_t	Fork(void);
pid_t wait (int *status);
pid_t	Waitpid(pid_t pid, int *status, int options);
void printbanner(void);
void dbzSpinnerLoading(void);
void exitLoading();
char *Readline(void);
void init_signals(void);

void *Malloc (size_t size, const char *name);
void *Realloc (void *ptr, size_t size, const char *name);
char	*ft_strdup(const char *src, const char *name);
void free_argv(char **argv, const char *name);
void free_envp(char **envp, const char *name);
void free_env(t_env *env_list, const char *name);
void free_tokens(t_tokens *head, const char *name);
void Free(void *ptr, const char *name);
void check_memory_leaks(void);

char* ft_strncpy(char* dest, const char* src, size_t n);
size_t	ft_strlen(const char *str);
int	ft_strcmp(const char *s1, const char *s2);
void	*ft_memcpy(void *dest, const void *src, size_t n);
char* ft_strtok(char* input_string, const char* delimiter);
int ft_strncmp(const char* str1, const char* str2, size_t n);
int	ft_isalpha(int c);
char	*ft_strchr(const char *str, int search_str);


int	ft_check(char c);
void add_token(t_tokens **head, char *token, int token_num, t_token_type type);
void skip_whitespace(char *input, size_t len, t_parse *ctx);
// t_tokens	*input_parse(char *input);
t_tokens	*input_parse2(char *input);

t_env	*create_env_node(const char *key, const char *value);
t_env	*init_env(char **envp);
char	*get_env_value(const char *key, t_env *env_list);
char **tokens_to_args(t_tokens *tokens_head);

/* builtins list*/
int builtin_cd(t_tokens *tokens, t_env **env_list);
int builtin_echo(t_tokens *tokens, t_env **env_list);
// int builtin_env(t_env *env_list);
int builtin_env(t_tokens *tokens, t_env **env_list);
int builtin_export(t_tokens *tokens, t_env **env_list);
int builtin_pwd(t_tokens *tokens, t_env **env_list);
int builtin_unset(t_tokens *tokens, t_env **env_list);
void builtin_exit(t_tokens *tokens, t_env **env_list, char *inp_line);


char **tokens_to_array(t_tokens *tokens);
int is_builtin(t_tokens **tokens);
int execute_builtin(t_tokens *tokens, t_env **env_list, int last_status, char *inp_line);
int executor(char** args, char** env);



#endif
/*#echo "Hello $echo "Hello $USER" | grep Hello > out.txt; cat < out.txt && export VAR="42"*/