#include "../minishell.h"

void	prepare_str(t_t *t, t_t **token_list)
{
	char	*str_quote;

	str_quote = malloc((t->pos - t->anchor_pos) + 1);
	ft_strlcpy(str_quote, t->input + t->anchor_pos,
		(t->pos - t->anchor_pos) + 1);
	if (t->input[t->pos +1] == t->input[t->quote])
		temp_token(t, str_quote);
	else
	{
		if (t->tmp_token && (!t->input[t->pos +1] || t->input[t->pos +1] == ' '
				|| t->input[t->pos +1] == '<' || t->input[t->pos +1] == '>'))
			last_str(t, str_quote, token_list);
		else
		{
			if (t->input[t->pos +1] && t->input[t->pos +1] != ' '
				&& t->input[t->pos +1] != '<' && t->input[t->pos +1] != '>')
				temp_token(t, str_quote);
			else
			{
				add_custom_token(str_quote, TOKEN_WORD, token_list);
				free(str_quote);
			}
		}
	}
}

void	temp_token(t_t *t, char *str)
{
	char	*tmp;

	tmp = NULL;
	if (!t->tmp_token)
	{
		t->tmp_token = ft_strdup(str);
		free(str);
		return ;
	}
	tmp = ft_strjoin(t->tmp_token, str);
	free(t->tmp_token);
	t->tmp_token = ft_strdup(tmp);
	free(tmp);
	free(str);
}

void	add_token_2(t_t *new_token, t_t **token_list, int redir_control, t_t *t)
{
	t_t	*last;

	new_token->next = NULL;
	if (*token_list == NULL)
	{
		*token_list = new_token;
	}
	else
	{
		last = *token_list;
		while (last->next != NULL)
			last = last->next;
		last->next = new_token;
	}
	if (redir_control == 1)
		t->pos--;
}

void	handle_first_quote_case(t_t *t, bool *free_input)
{
	char	*begin_quote;
	char	*after_quote;
	char	*end_str;

	begin_quote = malloc(t->quote + 1);
	after_quote = NULL;
	end_str = NULL;
	ft_strlcpy(begin_quote, t->input, t->quote + 1);
	if (t->input[t->pos])
		t->pos++;
	while (t->input[t->pos])
		t->pos++;
	after_quote = malloc((t->pos - t->quote + 1) + 1);
	ft_strlcpy(after_quote, t->input + (t->quote + 2),
		t->pos - (t->quote));
	end_str = ft_strjoin(begin_quote, after_quote);
	free(begin_quote);
	free(after_quote);
	free(t->input);
	t->input = ft_strdup(end_str);
	t->start = t->input;
	free(end_str);
	t->pos = t->anchor_pos;
	*free_input = 1;
}

void	handle_middle_quote_case(t_t *t, t_t **token_list)
{
	char	*begin_quote;
	char	*after_quote;
	char	*end_str;

	begin_quote = malloc((t->quote - t->anchor_pos) + 1);
	after_quote = NULL;
	end_str = NULL;
	ft_strlcpy(begin_quote, t->input + t->anchor_pos,
		(t->quote - t->anchor_pos) + 1);
	after_quote = malloc((t->pos - t->quote) + 1);
	ft_strlcpy(after_quote, t->input + (t->quote + 1),
		t->pos - (t->quote));
	end_str = create_end_str(begin_quote, after_quote);
	if (t->input[t->pos + 1] && (t->input[t->pos + 1] != ' '
			|| t->input[t->pos + 1] == t->input[t->quote]))
		temp_token(t, end_str);
	else if (t->tmp_token)
		last_str(t, end_str, token_list);
	else
	{
		add_custom_token(end_str, TOKEN_WORD, token_list);
		free(end_str);
	}
	if (t->input[t->pos])
		t->pos++;
}
