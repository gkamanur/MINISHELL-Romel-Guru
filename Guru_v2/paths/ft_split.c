/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:48:30 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/06 13:48:36 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_wordcount(const char *s, char c)
{
	int	count;

	if (!s)
		return (0);
	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			count++;
			while (*s && *s != c)
				s++;
		}
	}
	return (count);
}

int	ft_wordlen(const char *s, char c)
{
	int	len;

	if (!s)
		return (0);
	len = 0;
	while (s[len] && s[len] != c)
		len++;
	return (len);
}

void	free_split(char **result)
{
	int	i;

	if (!result)
		return ;
	i = 0;
	while (result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t n)
{
	size_t	i;
	size_t	src_len;

	if (!dest || !src)
		return (0);
	src_len = 0;
	while (src[src_len])
		src_len++;
	i = 0;
	if (n > 0)
	{
		while (i < n - 1 && src[i])
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
	}
	return (src_len);
}

char	**ft_split(const char *s, char c)
{
	char	**result;
	int		i;

	result = (char **)malloc((ft_wordcount(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			result[i] = (char *)malloc((ft_wordlen(s, c) + 1) * sizeof(char));
			if (!result[i])
			{
				free_split(result);
				return (NULL);
			}
			ft_strlcpy(result[i++], s, ft_wordlen(s, c) + 1);
			s += ft_wordlen(s, c);
		}
	}
	result[i] = NULL;
	return (result);
}
