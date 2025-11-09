#include "../minishell.h"
#include "../libft/libft.h"

void	quotes(t_t *t)
{
	if ((t->input[t->pos] == '\'' && !t->double_quote) && (t->start == t->input
			|| t->input[t->pos - 1] != '\\'))
	{
		t->single_quote = !t->single_quote;
		if (t->input[t->anchor_pos] == ' ')
			t->anchor_pos++;
		t->quote = t->pos;
		if (t->input[t->pos])
			t->pos++;
	}
	else if ((t->input[t->pos] == '\"' && !t->single_quote)
		&& (t->start == t->input || t->input[t->pos - 1] != '\\'))
	{
		t->double_quote = !t->double_quote;
		if (t->input[t->anchor_pos] == ' ')
			t->anchor_pos++;
		t->quote = t->pos;
		if (t->input[t->pos])
			t->pos++;
	}
}

void	one_less_more(t_t *t, t_t **token_list)
{
	if (t->pos == t->anchor_pos)
		t->pos++;
	add_token(t, token_list);
}

void	two_less_more(t_t *t, t_t **token_list)
{
	char	*begin_str;

	begin_str = NULL;
	if (t->input[t->anchor_pos] != ' ' && t->input[t->anchor_pos] != '<'
		&& t->input[t->anchor_pos] != '>')
	{
		begin_str = malloc((t->pos - t->anchor_pos) + 1);
		ft_strlcpy(begin_str, t->input + t->anchor_pos,
			t->pos - t->anchor_pos + 1);
		add_custom_token(begin_str, TOKEN_WORD, token_list);
		t->anchor_pos = t->pos;
	}
	t->pos += 2;
	add_token(t, token_list);
	t->anchor_pos = t->pos;
}

void	metacharacters(t_t *t, t_t **token_list)
{
	if (t->input[t->pos])
	{
		if (t->input[t->pos] == ' ' || t->input[t->pos] == '|')
			check_pipes(t, token_list);
		else if ((t->input[t->pos] == '<' && t->input[t->pos +1] != '<')
			|| (t->input[t->pos] == '>' && t->input[t->pos +1] != '>'))
			one_less_more(t, token_list);
		else if ((t->input[t->pos] == '<' && t->input[t->pos +1] == '<'
				&& t->input[t->pos +2] != '<') || (t->input[t->pos] == '>'
				&& t->input[t->pos +1] == '>' && t->input[t->pos +2] != '>'))
			two_less_more(t, token_list);
		else
			triple_meta(t, token_list);
		if ((t->input[t->pos] == '<' || t->input[t->pos] == '>')
			&& t->input[t->pos -1] != ' ')
			return ;
		if (t->input[t->pos] != '"' && t->input[t->pos] != '\'')
			t->pos++;
	}
}

void	open_quotes(t_t *t, t_t **token_list, bool *free_input)
{
	if (t->single_quote || t->double_quote)
	{
		if (t->double_quote)
			check_var(t);
		while (t->input[t->pos])
		{
			if (t->single_quote && t->input[t->pos] == '\''
				&& t->input[t->pos - 1] != '\\')
			{
				handle_single_quote(t, token_list, free_input);
				return ;
			}
			else if (t->double_quote && t->input[t->pos] == '\"'
				&& t->input[t->pos - 1] != '\\')
			{
				handle_double_quote(t, token_list, free_input);
				return ;
			}
			if (t->input[t->pos])
				t->pos++;
		}
	}
	if (t->input[t->pos])
		t->pos++;
}
