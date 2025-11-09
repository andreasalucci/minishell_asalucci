#include "../minishell.h"

void	prepare_quotes(t_t *t, t_t **token_list, bool *free_input)
{
	char	*begin_quote;

	begin_quote = NULL;
	if (t->pos > t->quote)
	{
		if (t->pos == t->quote + 1)
			handle_first_quote_case(t, free_input);
		else
			handle_middle_quote_case(t, token_list);
	}
	else
	{
		begin_quote = malloc(t->quote + 1);
		ft_strlcpy(begin_quote, t->input, t->quote + 1);
		free(t->input);
		t->input = ft_strdup(begin_quote);
		free(begin_quote);
		*free_input = 1;
	}
	t->anchor_pos = t->pos;
}

static int	skip_spaces_and_check_redir(t_t *t, int *redir_control)
{
	*redir_control = 0;
	while (t->input[t->anchor_pos] == ' ' && t->anchor_pos < t->pos)
		t->anchor_pos++;
	if (t->anchor_pos == t->pos
		&& (t->input[t->pos] == '<' || t->input[t->pos] == '>'))
	{
		if (t->input[t->pos + 1] == '<' || t->input[t->pos + 1] == '>')
			return (0);
		if (t->input[t->pos])
			t->pos++;
		*redir_control = 1;
	}
	return (1);
}

static int	create_new_token(t_t *t, t_t **new_token, size_t len)
{
	int	check_memory;

	check_memory = alloc_new_token(new_token, len);
	if (check_memory == 0)
		return (0);
	ft_strlcpy((*new_token)->value, t->start + t->anchor_pos, len + 1);
	return (1);
}

static int	handle_tmp_token_case(t_t *t, t_t **token_list, t_t *new_token)
{
	if (t->tmp_token)
	{
		last_str(t, ft_strdup(new_token->value), token_list);
		free(new_token->value);
		free(new_token);
		t->anchor_pos = t->pos;
		return (1);
	}
	return (0);
}

void	add_token(t_t *t, t_t **token_list)
{
	size_t	len;
	t_t		*new_token;
	int		redir_control;

	if (t->pos > ft_strlen(t->input))
		return ;
	if (!skip_spaces_and_check_redir(t, &redir_control))
		return ;
	len = t->pos - t->anchor_pos;
	if (len == 0 || t->input[t->anchor_pos] == ' ')
	{
		t->anchor_pos++;
		return ;
	}
	if (!create_new_token(t, &new_token, len))
		return ;
	if (handle_tmp_token_case(t, token_list, new_token))
		return ;
	t->anchor_pos = t->pos;
	if (ft_strchr("|<>", new_token->value[0]))
		new_token->type = METACHAR;
	else
		new_token->type = TOKEN_WORD;
	new_token->error = false;
	add_token_2(new_token, token_list, redir_control, t);
}
