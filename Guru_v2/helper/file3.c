/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/30 14:54:12 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 12:37:51 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void *ft_memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    
    if (!dest || !src)
        return dest;
        
    while (n--)
        *d++ = *s++;
        
    return dest;
}

char *ft_strtok(char *input_string, const char *delimiter)
{
    static char *last_token = NULL;
    char *token_start;
    
    if (input_string != NULL)
        last_token = input_string;
    else if (last_token == NULL)
        return NULL;
        
    // Skip leading delimiters
    while (*last_token && ft_strchr(delimiter, *last_token))
        last_token++;
        
    if (*last_token == '\0')
    {
        last_token = NULL;
        return NULL;
    }
    
    token_start = last_token;
    
    // Find end of token
    while (*last_token && !ft_strchr(delimiter, *last_token))
        last_token++;
        
    if (*last_token)
    {
        *last_token = '\0';
        last_token++;
    }
    else
        last_token = NULL;
        
    return token_start;
}

int ft_strncmp(const char *str1, const char *str2, size_t n)
{
    if (!str1 || !str2)
        return (str1 == str2) ? 0 : (str1 ? 1 : -1);
        
    while (n > 0 && *str1 && *str2 && *str1 == *str2)
    {
        str1++;
        str2++;
        n--;
    }
    
    if (n == 0)
        return 0;
        
    return (unsigned char)*str1 - (unsigned char)*str2;
}

int ft_isalpha(int c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}

char *ft_strchr(const char *str, int search_str)
{
    if (!str)
        return NULL;
        
    while (*str)
    {
        if (*str == search_str)
            return (char *)str;
        str++;
    }
    
    if (search_str == '\0')
        return (char *)str;
        
    return NULL;
}

char *ft_strncpy(char *dest, const char *src, size_t n)
{
    size_t i = 0;
    
    if (!dest || !src)
        return dest;
        
    while (i < n && src[i])
    {
        dest[i] = src[i];
        i++;
    }
    
    while (i < n)
    {
        dest[i] = '\0';
        i++;
    }
    
    return dest;
}