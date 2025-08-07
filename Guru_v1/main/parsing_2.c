/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkamanur <gkamanur@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 13:09:29 by gkamanur          #+#    #+#             */
/*   Updated: 2025/08/06 11:06:53 by gkamanur         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_token_type get_token_type(const char *token)
{
	if (strcmp(token, "|") == 0)
		return TOKEN_PIPE;
	if (strcmp(token, "<") == 0)
		return TOKEN_REDIRECT_IN;
	if (strcmp(token, ">") == 0)
		return TOKEN_REDIRECT_OUT;
	if (strcmp(token, ">>") == 0)
		return TOKEN_REDIRECT_APPEND;
	if (strcmp(token, "<<") == 0)
		return TOKEN_HEREDOC;
	if (strcmp(token, "&&") == 0)
		return TOKEN_AND;
	if (strcmp(token, "||") == 0)
		return TOKEN_OR;
	if (strcmp(token, ";") == 0)
		return TOKEN_SEMICOLON;
	if (strcmp(token, "&") == 0)
		return TOKEN_BACKGROUND;
	if (strcmp(token, "(") == 0)
		return TOKEN_OPEN_PAREN;
	if (strcmp(token, ")") == 0)
		return TOKEN_CLOSE_PAREN;

	// New: Detect assignments like VAR=42
	if (ft_strchr(token, '=') && ft_isalpha(token[0]))
		return TOKEN_ASSIGNMENT;

	return TOKEN_ARG;
}


void add_token(t_tokens **head, char *token, int token_num, t_token_type type)
{
    t_tokens *new_node = Malloc(sizeof(t_tokens), "token_list_alloc");
    new_node->token = ft_strdup(token, "token_string_alloc");
    new_node->token_num = token_num;
    new_node->type = type;
    new_node->next = NULL;

    if (*head == NULL)
        *head = new_node;
    else
    {
        t_tokens *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_node;
    }
}

void skip_whitespace(char *input, size_t len, t_parse *ctx)
{
	while (ctx->i < len && (ft_check(input[ctx->i]) == 1))
		ctx->i++;
}

int	is_control_operator(t_token_type type)
{
	return (type == TOKEN_AND || type == TOKEN_SEMICOLON || type == TOKEN_PIPE || type == TOKEN_OR);
}

int handle_quote(char *input, size_t len, t_parse *ctx)
{
	char	quote;
	size_t	buf_pos;
	int flag;

	buf_pos = 0;
	flag = 0;
	quote = input[ctx->i++];
	while (ctx->i < len && input[ctx->i] != quote)
		ctx->buffer[buf_pos++] = input[ctx->i++];
	if (ctx->i < len && input[ctx->i] == quote)
	{
		ctx->i++;
		ctx->buffer[buf_pos] = '\0';
		return (1);
	}
	return (0);
}

static void handle_squoted_string(char *input, size_t len, t_parse *ctx)
{
	if (handle_quote(input, len, ctx))
		add_token(ctx->head, ctx->buffer, ctx->token_num++, TOKEN_SQUOTED);
	else
		putstr_clr("incomplete single quotes", 2, RED);
}

static void handle_dquoted_string(char *input, size_t len, t_parse *ctx)
{
	if (handle_quote(input, len, ctx))
		add_token(ctx->head, ctx->buffer, ctx->token_num++, TOKEN_DQUOTED);
	else
		putstr_clr("incomplete double quotes", 2, RED);
}



static void	handle_variable(char *input, size_t len, t_parse *ctx)
{
	size_t	buf_pos;

	buf_pos = 0;
	ctx->buffer[buf_pos++] = input[ctx->i++];
	while (ctx->i < len && (ft_check(input[ctx->i]) == 5 || input[ctx->i] == '_'))
		ctx->buffer[buf_pos++] = input[ctx->i++];
	ctx->buffer[buf_pos] = '\0';
	add_token(ctx->head, ctx->buffer, ctx->token_num++, TOKEN_VARIABLE);
}

static void handle_operator_or_word(char *input, size_t len, t_parse *ctx)
{
	size_t buf_pos = 0;
	size_t buffer_size = BUFSIZEE;

	// Two-character operators like >>, <<, &&, ||
	if (ctx->i + 1 < len && is_operator(&input[ctx->i]) && input[ctx->i] == input[ctx->i + 1])
	{
		ctx->buffer[buf_pos++] = input[ctx->i++];
		ctx->buffer[buf_pos++] = input[ctx->i++];
	}
	else if (is_operator(&input[ctx->i]))
	{
		ctx->buffer[buf_pos++] = input[ctx->i++];
	}
	else
	{
		while (ctx->i < len && !is_special_char(input[ctx->i]) && !is_operator(&input[ctx->i]))
		{
			if (buf_pos + 1 >= buffer_size)
			{
				buffer_size *= 2;
				ctx->buffer = Realloc(ctx->buffer, buffer_size, "buffer_realloc");
			}
			ctx->buffer[buf_pos++] = input[ctx->i++];
		}

		// Special fix: split tokens like `out.txt;` → `out.txt`, `;`
		if (buf_pos > 0 && is_operator(&ctx->buffer[buf_pos - 1]))
		{
			buf_pos--;
			ctx->i--;
		}
	}

	ctx->buffer[buf_pos] = '\0';

	if (buf_pos > 0)
	{
		t_token_type type = get_token_type(ctx->buffer);

		if (type == TOKEN_ARG)
		{
			t_tokens *last = *ctx->head;
			while (last && last->next)
				last = last->next;

			// First token or after a control operator = it's a CMD
			if (ctx->token_num == 0 || (last && is_control_operator(last->type)))
				type = TOKEN_CMD;
		}
		add_token(ctx->head, ctx->buffer, ctx->token_num++, type);
	}
}


t_tokens	*input_parse2(char *input)
{
	t_parse	ctx;
	t_tokens	*head;
	char		*buffer;
	size_t		len;

	if (!input)
		return (NULL);
	len = strlen(input);
	buffer = Malloc(BUFSIZEE, "buf_input_parse");
	head = NULL;
	ctx.head = &head;
	ctx.buffer = buffer;
	ctx.i = 0;
	ctx.token_num = 0;
	while (ctx.i < len)
	{
		skip_whitespace(input, len, &ctx);
		if (ctx.i >= len)
			break ;
		if (ft_check(input[ctx.i]) == 2)
			handle_squoted_string(input, len, &ctx);
		else if (ft_check(input[ctx.i]) == 3)
			handle_dquoted_string(input, len, &ctx);
		else if (ft_check(input[ctx.i]) == 4)
			handle_variable(input, len, &ctx);
		else
			handle_operator_or_word(input, len, &ctx);
	}
	Free(buffer, "buffer_input_parse");
	return (head);
}
