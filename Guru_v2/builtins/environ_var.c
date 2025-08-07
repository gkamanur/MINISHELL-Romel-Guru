/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 19:51:09 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/05 09:58:53 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env *create_env_node(const char *key, const char *value) {
    t_env *node = Malloc(sizeof(t_env), "env_node");
    if (!node) return NULL;
    
    node->key = ft_strdup(key, "env_key");
    node->value = ft_strdup(value, "env_value");
    
    if (!node->key || !node->value) {
        Free(node->key, "env_key_cleanup");
        Free(node->value, "env_value_cleanup");
        Free(node, "env_node_cleanup");
        return NULL;
    }
    node->next = NULL;
    return node;
}

t_env *init_env(char **envp) {
    t_env *env_list = NULL;
    for (int i = 0; envp[i]; i++) {
        char *entry = ft_strdup(envp[i], "env_entry");
        if (!entry) continue;
        
        char *eq = ft_strchr(entry, '=');
        if (eq) {
            *eq = '\0';
            t_env *node = create_env_node(entry, eq + 1);
            if (node) {
                node->next = env_list;
                env_list = node;
            }
        }
        Free(entry, "env_entry");
    }
    return env_list;
}

char *get_env_value(const char *key, t_env *env_list) {
    t_env *current = env_list;
    while (current) {
        if (ft_strcmp(current->key, key) == 0)
            return current->value;
        current = current->next;
    }
    return "";
}
