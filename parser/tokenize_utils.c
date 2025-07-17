#include "../minishell.h"
#include "../libft/libft.h"

void quotes(t_t *t)
{
	if ((t->input[t->pos] == '\'' && !t->double_quote) && (t->start == t->input || t->input[t->pos - 1] != '\\'))
	{
		t->single_quote = !t->single_quote;
		if (t->input[t->anchor_pos] == ' ')
			t->anchor_pos++;
		t->quote = t->pos; // marca la virgoletta
		t->pos++;
	}
	else if ((t->input[t->pos] == '\"' && !t->single_quote) && (t->start == t->input || t->input[t->pos - 1] != '\\'))
	{
		
		t->double_quote = !t->double_quote;
		if (t->input[t->anchor_pos] == ' ')
			t->anchor_pos++;
		t->quote = t->pos; // marca la virgoletta
		t->pos++;
	}
}

// void	quotes(t_t *t)
// {
// 	if ((t->input[t->pos] == '\'' && !t->double_quote) && (t->start == t->input
// 		|| t->input[t->pos -1] != '\\'))
// 	{
// 		t->single_quote = !t->single_quote;
// 		t->pos++;
// 	}
// 	else if ((t->input[t->pos] == '\"' && !t->single_quote) && (t->start == t->input
// 		|| t->input[t->pos -1] != '\\'))
// 	{
// 		t->double_quote = !t->double_quote;
// 		t->pos++;
// 	}
// }

void	metacharacters(t_t *t, t_t **token_list)
{
	
	if (t->input[t->pos])
	{
		
		if (t->input[t->pos] == ' ' || t->input[t->pos] == '|')
			check_pipes(t, token_list);
		else if (t->input[t->pos] == '<' && t->input[t->pos +1] != '<')
		{
			if (t->pos == t->anchor_pos)
				t->pos++;
			add_token(t, token_list);
		}
				
		else if (t->input[t->pos] == '>' && t->input[t->pos +1] != '>')
		{
			if (t->pos == t->anchor_pos)
				t->pos++;
			add_token(t, token_list);
		}
		else if (t->input[t->pos] == '<' && t->input[t->pos +1] == '<'
		&& t->input[t->pos +2] != '<' )
			{
				t->pos += 2;
				add_token(t, token_list);
				t->anchor_pos = t->pos +1;
			}
		else if (t->input[t->pos] == '>' && t->input[t->pos +1] == '>'
		&& t->input[t->pos +2] != '>')
			{
				t->pos += 2;
				add_token(t, token_list);
				t->anchor_pos = t->pos +1;
			}
		else
			triple_meta(t, token_list);
		t->pos++;
	}
}

void open_quotes(t_t *t, t_t **token_list)
{
	char	*begin_quote;
	char	*after_quote;
	char	*end_str;
	if (t->single_quote || t->double_quote)
	{
		
		while (t->input[t->pos])
		{
			if (t->single_quote && t->input[t->pos] == '\'' && t->input[t->pos - 1] != '\\')
			{
				t->single_quote = !t->single_quote;
				if (t->pos > t->anchor_pos) 
				{
					if (t->quote != t->anchor_pos -1) // per che non entre in comandi tipo -"vmaos"-
					{
						begin_quote = malloc((t->quote - t->anchor_pos) +1);
						ft_strlcpy(begin_quote, t->input + t->anchor_pos, (t->quote - t->anchor_pos) +1);
						after_quote = malloc((t->pos - t->quote));
						ft_strlcpy(after_quote, t->input + (t->quote +1), t->pos - t->quote);
						end_str =ft_strjoin(begin_quote, after_quote);
						add_custom_token(end_str, TOKEN_WORD, token_list);
					}
					else
						add_token(t, token_list);
				}
				t->anchor_pos = t->pos + 1;  // Salta la virgoletta finale
				t->pos++;
				return;
			}
			else if (t->double_quote && t->input[t->pos] == '\"' && t->input[t->pos - 1] != '\\')
			{
				t->double_quote = !t->double_quote;
				if (t->pos > t->anchor_pos) 
				{
					
					if (t->quote != t->anchor_pos - 1) // per che non entre in comandi tipo -"vmaos"-
					{
						
						begin_quote = malloc((t->quote - t->anchor_pos) +1);
						ft_strlcpy(begin_quote, t->input + t->anchor_pos, (t->quote - t->anchor_pos) +1);
						after_quote = malloc((t->pos - t->quote));
						ft_strlcpy(after_quote, t->input + (t->quote +1), t->pos - t->quote);
						end_str =ft_strjoin(begin_quote, after_quote);
						add_custom_token(end_str, TOKEN_WORD, token_list);
					}
					else
					{
						
						add_token(t, token_list);
					}
						
				}
				t->anchor_pos = t->pos + 1;  // Salta la virgoletta finale
				t->pos++;
				return;
			}
			t->pos++;
		}
	}
	t->pos++;
}

// void	open_quotes(t_t *t, t_t **token_list)
// {
// 	if (t->single_quote || t->double_quote)
// 		while (t->input[t->pos])
// 		{
// 			if (t->single_quote && t->input[t->pos] == '\''&& t->input[t->pos -1] != '\\')
// 			{
// 				t->single_quote = !t->single_quote;
// 				t->anchor_pos++;
// 				add_token(t, token_list);
// 				t->pos += 2;
// 				t->anchor_pos = t->pos;
// 				return ;
// 			}
// 			else if (t->double_quote && t->input[t->pos] == '\"'&& t->input[t->pos -1] != '\\')
// 			{
// 				t->double_quote = !t->double_quote;
// 				t->anchor_pos++;
// 				add_token(t, token_list);
// 				t->pos += 2;
// 				t->anchor_pos = t->pos;
// 				return ;
// 			}
// 			t->pos++;
// 		}
// 	t->pos++;
// }

void add_token(t_t *t, t_t **token_list)
{
	if (t->pos > ft_strlen(t->input))
		return ;
	size_t	len;
	t_t *new_token;
	int check_memory;
	while (t->input[t->anchor_pos] == ' ' && t->anchor_pos < t->pos)
		t->anchor_pos++;
	if(t->anchor_pos == t->pos && (t->input[t->pos] == '<' || t->input[t->pos] == '>'))
		return ;
	len = t->pos - t->anchor_pos;
	if (len == 0 || t->input[t->anchor_pos] == ' ')
	{
		t->anchor_pos++;
		return;
	}
	check_memory = alloc_new_token(&new_token, len);
	if (check_memory == 0)
		return ;
	ft_strlcpy(new_token->value, t->start + t->anchor_pos, len +1);
	t->anchor_pos = t->pos;	// (il +1 è per non ripettere l'ultimo carattere) in qualche momento funcionava adesso non piu, tolto.
	if (ft_strchr(("|<>"), new_token->value[0]))
	{
		new_token->type = METACHAR;
		new_token->error = false;
	}
		
	else
	{
		new_token->type = TOKEN_WORD;
		new_token->error = false;
	}
	add_token_2(new_token, token_list);
}

void add_token_2(t_t *new_token, t_t **token_list)
{
	new_token->next = NULL;
	if (*token_list == NULL) {
		*token_list = new_token;
	} else {
		t_t *last = *token_list;
		while (last->next != NULL)
			last = last->next;
		last->next = new_token;
	}
}