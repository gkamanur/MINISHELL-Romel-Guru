/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/29 14:03:51 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/05 15:56:38 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int malloc_count = 1; // 1 for readline
int free_count = 0;


void *Malloc(size_t size, const char *name) {
    (void) name;
    if (size == 0)
        return NULL;
    void *ptr = malloc(size);
    if (!ptr) {
        putstr_clr("Malloc failed", 2, RED);
        exit(EXIT_FAILURE);
    }
    malloc_count++;
    // printf("malloc: %s (%d)\n", name, malloc_count);
    return ptr;
}

void *Realloc(void *ptr, size_t size, const char *name) {
    (void) name;
    void *new_ptr = realloc(ptr, size);
    if (!new_ptr && size != 0) {
        putstr_clr("Realloc failed", 2, RED);
        exit(EXIT_FAILURE);
    }
    malloc_count++;
    // printf("realloc: %s (%d)\n", name, malloc_count);
    return new_ptr;
}

char *ft_strdup(const char *src, const char *name) {
    (void) name;
    size_t len = strlen(src);
    char *copy = malloc(len + 1);
    if (!copy)
        return NULL;
    strcpy(copy, src);
    malloc_count++;
    // printf("strdup: %s (%d)\n", name, malloc_count);
    return copy;
}

// -------------------- Free Wrapper --------------------

void Free(void *ptr, const char *name) {
    (void) name;
    if (ptr) {
        free(ptr);
        free_count++;
        // printf("free: %s (%d)\n", name, free_count);
    }
}

// -------------------- Free Functions --------------------

void free_tokens(t_tokens *head, const char *name) {
    (void) name;
    t_tokens *temp;
    while (head) {
        temp = head;
        head = head->next;
        Free(temp->token, "token");
        Free(temp, "t_tokens");
    }
    // printf("freed all tokens (%s)\n", name);
}

void free_env(t_env *env_list, const char *name) {
    (void) name;
    while (env_list) {
        t_env *tmp = env_list;
        env_list = env_list->next;
        Free(tmp->key, "env.key");
        Free(tmp->value, "env.value");
        Free(tmp, "env struct");
    }
    // printf("freed all envs (%s)\n", name);
}

void free_envp(char **envp, const char *name) {
    (void) name;
    for (int i = 0; envp[i]; i++)
        Free(envp[i], "envp[i]");
    Free(envp, "envp");
    // printf("freed envp (%s)\n", name);
}

void free_argv(char **argv, const char *name) {
    (void) name;
    for (int i = 0; argv[i]; i++)
        Free(argv[i], "argv[i]");
    Free(argv, "argv");
    // printf("freed argv (%s)\n", name);
}

void check_memory_leaks() {
    printf("\n=== MEMORY TRACKING ===\n");
    printf("Total mallocs : %d\n", malloc_count);
    printf("Total frees   : %d\n", free_count);
    if (malloc_count != free_count) {
        printf(RED "💥 Memory leak detected: %d not freed\n\033[0m", malloc_count - free_count);
    } else {
        printf("✅ All memory freed properly.\n");
    }
}
