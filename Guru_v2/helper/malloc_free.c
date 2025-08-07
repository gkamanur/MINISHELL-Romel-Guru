/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:03:51 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:39:40 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int malloc_count = 0;
static int free_count = 0;

void *Malloc(size_t size, const char *name)
{
    void *ptr = malloc(size);
    if (!ptr)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    malloc_count++;
    return ptr;
}

void *Realloc(void *ptr, size_t size, const char *name)
{
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr && size > 0)
    {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    return new_ptr;
}

char *ft_strdup(const char *src, const char *name)
{
    char *dup;
    size_t len;
    size_t i = 0;
    
    if (!src)
        return NULL;
        
    len = ft_strlen(src);
    dup = Malloc(len + 1, name);
    
    while (i < len)
    {
        dup[i] = src[i];
        i++;
    }
    dup[i] = '\0';
    
    return dup;
}

void Free(void *ptr, const char *name)
{
    if (ptr)
    {
        free(ptr);
        free_count++;
    }
}

void free_argv(char **argv, const char *name)
{
    int i = 0;
    
    if (!argv)
        return;
        
    while (argv[i])
    {
        Free(argv[i], name);
        i++;
    }
    Free(argv, name);
}

void free_envp(char **envp, const char *name)
{
    free_argv(envp, name);
}

void free_env(t_env *env_list, const char *name)
{
    t_env *temp;
    
    while (env_list)
    {
        temp = env_list;
        env_list = env_list->next;
        Free(temp->key, name);
        Free(temp->value, name);
        Free(temp, name);
    }
}

void free_tokens(t_tokens *head, const char *name)
{
    t_tokens *temp;
    
    while (head)
    {
        temp = head;
        head = head->next;
        Free(temp->token, name);
        Free(temp, name);
    }
}

void check_memory_leaks(void)
{
    printf("Memory allocation summary:\n");
    printf("Malloc calls: %d\n", malloc_count);
    printf("Free calls: %d\n", free_count);
    if (malloc_count != free_count)
        printf("WARNING: Potential memory leak detected!\n");
    else
        printf("No memory leaks detected.\n");
}
