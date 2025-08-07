/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 13:41:08 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/07 15:38:53 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_check(char c)
{
    if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r')
        return 1; // Whitespace
    if (c == '\'')
        return 2; // Single quote
    if (c == '"')
        return 3; // Double quote
    if (c == '$')
        return 4; // Variable
    if (ft_isalpha(c) || (c >= '0' && c <= '9'))
        return 5; // Alphanumeric
    return 0; // Other
}

int	is_operator(const char *str)
{
	const char	*operators[] = {
		"|", ">", "<", ">>", "<<", "&&", "||", NULL
	};
	int			i;

	i = 0;
	while (operators[i])
	{
		if (strcmp(str, operators[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	is_special_char(int c)
{
	const char	specials[] = {' ', '\t', '\n', '\v', '\f',
							'\r', '\a', '"', '\'', '$', '\0'};
	int			i;

	i = 0;
	while (specials[i])
	{
		if (c == specials[i])
			return (1);
		i++;
	}
	return (0);
}



