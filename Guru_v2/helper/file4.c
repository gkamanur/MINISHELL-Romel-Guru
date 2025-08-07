/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file4.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 12:43:54 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 16:13:19 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Checks if character is alphanumeric (letter or digit)
int ft_isalnum(int c)
{
    return (isalpha(c) || isdigit(c));
}

// Extracts substring from string s starting at index start with length len
char *ft_substr(char const *s, unsigned int start, size_t len)
{
    char *substr;
    size_t i;
    size_t slen;

    if (!s)
        return (NULL);
    slen = 0;
    while (s[slen])
        slen++;
    if (start >= slen)
        return (calloc(1, sizeof(char)));
    if (len > slen - start)
        len = slen - start;
    substr = (char *)malloc((len + 1) * sizeof(char));
    if (!substr)
        return (NULL);
    i = 0;
    while (i < len && s[start + i])
    {
        substr[i] = s[start + i];
        i++;
    }
    substr[i] = '\0';
    return (substr);
}

// Converts integer to string representation
char *ft_itoa(int n)
{
    char *str;
    size_t len;
    long nb;
    int is_negative;

    nb = n;
    len = (nb <= 0) ? 1 : 0;
    is_negative = (nb < 0) ? 1 : 0;
    while (n)
    {
        len++;
        n /= 10;
    }
    str = (char *)malloc((len + 1) * sizeof(char));
    if (!str)
        return (NULL);
    str[len] = '\0';
    if (nb == 0)
        str[0] = '0';
    while (nb)
    {
        str[--len] = (is_negative ? -(nb % 10) : (nb % 10)) + '0';
        nb /= 10;
    }
    if (is_negative)
        str[0] = '-';
    return (str);
}

// Joins two strings into a new string
char *ft_strjoin(char const *s1, char const *s2)
{
    char *result;
    size_t len1;
    size_t len2;
    size_t i;

    if (!s1 || !s2)
        return (NULL);
    len1 = 0;
    while (s1[len1])
        len1++;
    len2 = 0;
    while (s2[len2])
        len2++;
    result = (char *)malloc((len1 + len2 + 1) * sizeof(char));
    if (!result)
        return (NULL);
    i = 0;
    while (i < len1)
    {
        result[i] = s1[i];
        i++;
    }
    while (i < len1 + len2)
    {
        result[i] = s2[i - len1];
        i++;
    }
    result[i] = '\0';
    return (result);
}

char	*ft_strcat(char *dest, const char *src)
{
	char	*dest_ptr;

	if (!dest || !src)
		return (dest);
	dest_ptr = dest;
	while (*dest_ptr)
		dest_ptr++;
	while (*src)
	{
		*dest_ptr = *src;
		dest_ptr++;
		src++;
	}
	*dest_ptr = '\0';
	return (dest);
}