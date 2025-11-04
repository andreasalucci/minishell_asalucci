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
		if(t->input[t->pos])
			t->pos++;
	}
	else if ((t->input[t->pos] == '\"' && !t->single_quote) && (t->start == t->input || t->input[t->pos - 1] != '\\'))
	{
		t->double_quote = !t->double_quote;
		if (t->input[t->anchor_pos] == ' ')
			t->anchor_pos++;
		t->quote = t->pos; // marca la virgoletta
		if(t->input[t->pos])
			t->pos++;
	}
}

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
				t->anchor_pos = t->pos;
			}
		else if (t->input[t->pos] == '>' && t->input[t->pos +1] == '>'
		&& t->input[t->pos +2] != '>')
			{
				t->pos += 2;
				add_token(t, token_list);
				t->anchor_pos = t->pos;
			}
		else
			triple_meta(t, token_list);
		if ((t->input[t->pos] == '<' || t->input[t->pos] == '>') && t->input[t->pos -1] != ' ')
			return ;
		if (t->input[t->pos] != '"' && t->input[t->pos] != '\'')
			t->pos++;
	}
}

void open_quotes(t_t *t, t_t **token_list, bool *free_input)
{
	if (t->single_quote || t->double_quote)
	{
		if (t->double_quote)
			check_var(t);
		while (t->input[t->pos])
		{
			
			if (t->single_quote && t->input[t->pos] == '\'' && t->input[t->pos - 1] != '\\')
			{
				t->single_quote = !t->single_quote;
				if (t->pos > t->anchor_pos) 
				{
					if (t->quote != t->anchor_pos) // per che non entre in comandi tipo -"vmaos"-
					{
						prepare_quotes(t, token_list, free_input);
					}
					else
					{
						if (t->input[t->anchor_pos] == '\'')
							t->anchor_pos++;
						prepare_str(t, token_list);
						t->anchor_pos = t->pos + 1;
					}
				}
				if(t->input[t->pos])
					t->pos++;
				return;
			}
			else if (t->double_quote && t->input[t->pos] == '\"' && t->input[t->pos - 1] != '\\')
			{
				
				t->double_quote = !t->double_quote;
			
				if (t->pos > t->anchor_pos) 
				{
					
					if (t->quote != t->anchor_pos) // per che non entre in comandi tipo -"vmaos"-
						prepare_quotes(t, token_list, free_input);	
					else
					{
						
						if (t->input[t->anchor_pos] == '\"')
							t->anchor_pos++;
						prepare_str(t, token_list);
						if(t->input[t->pos])
							t->pos++;
						t->anchor_pos = t->pos;
						
					}	
				}
				if (t->input[t->pos] == '\"')
					t->continue_var = !t->continue_var;
				
				return;
			}
			if(t->input[t->pos])
				t->pos++;
		}
	}
	if(t->input[t->pos])
		t->pos++;
}

void prepare_quotes(t_t *t, t_t **token_list, bool *free_input)
{

	char	*begin_quote;
	char	*after_quote;
	char	*end_str;
	
	begin_quote = malloc(t->quote +1);
	after_quote = NULL;
	end_str = NULL;
	if (t->pos > t->quote)
	{
		if (t->pos == t->quote +1)
		{
			ft_strlcpy(begin_quote, t->input, t->quote +1);
			if (t->input[t->pos])
				t->pos++;
			while (t->input[t->pos])
				t->pos++;
			after_quote = malloc((t->pos - t->quote +1) +1);
			ft_strlcpy(after_quote, t->input + (t->quote +2), t->pos - (t->quote));
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
		else
		{
			ft_strlcpy(begin_quote, t->input + t->anchor_pos, (t->quote - t->anchor_pos) +1);
			after_quote = malloc((t->pos - t->quote) +1);
			ft_strlcpy(after_quote, t->input + (t->quote +1), t->pos - (t->quote));
			end_str = ft_strjoin(begin_quote, after_quote);
			free(begin_quote);
			free(after_quote);
			//ft_printf("t->pos:: %i, t->quote:: %i\n", t->pos, t->quote);
			if (t->input[t->pos +1] && (t->input[t->pos +1] != ' ' || t->input[t->pos +1] == t->input[t->quote]))
				temp_token(t, end_str);
			else
			{
				if (t->tmp_token)
					last_str(t, end_str, token_list);
				else
				{
					add_custom_token(end_str, TOKEN_WORD, token_list);
					free(end_str);
				}
			}
			if(t->input[t->pos])
				t->pos++;
		}
	}
	else
		{
			ft_strlcpy(begin_quote, t->input, t->quote +1);
			free(t->input);
			t->input = ft_strdup(begin_quote);
			free(begin_quote);
			*free_input = 1;
		}
	t->anchor_pos = t->pos;	
}
void	last_str(t_t *t, char *str, t_t **token_list)
{
	char	*end_str;
	
	end_str = NULL;

	end_str = ft_strjoin(t->tmp_token, str);
	add_custom_token(end_str, TOKEN_WORD, token_list);
	free(t->tmp_token);
	t->tmp_token = NULL;
	free(end_str);
	free(str);
}
void	temp_token(t_t *t, char *str)
{
	char	*tmp;
	
	tmp = NULL;
	if (!t->tmp_token)
	{
		t->tmp_token = ft_strdup(str);
		free(str);
		return;
	}
	tmp = ft_strjoin(t->tmp_token, str);
	free(t->tmp_token);
	t->tmp_token = ft_strdup(tmp);
	free(tmp);
	free(str);
	
}
void	prepare_str(t_t *t, t_t **token_list)
{
	
	char	*str_quote;

	str_quote = NULL;
	str_quote = malloc((t->pos - t->anchor_pos) +1);
	ft_strlcpy(str_quote, t->input + t->anchor_pos, (t->pos - t->anchor_pos) +1);
	
	if (t->input[t->pos +1] == t->input[t->quote])
		temp_token(t, str_quote);
	else
	{
		
		if (t->tmp_token && (!t->input[t->pos +1] || t->input[t->pos +1] == ' ' || t->input[t->pos +1] == '<' || t->input[t->pos +1] == '>'))
			last_str(t, str_quote, token_list);
		else
		{
			
			if (t->input[t->pos +1] && t->input[t->pos +1] != ' ' && t->input[t->pos +1] != '<' && t->input[t->pos +1] != '>')
				temp_token(t, str_quote);
			else
			{
				
				add_custom_token(str_quote, TOKEN_WORD, token_list);
				free(str_quote);
			}
				
		}
	}
}
void add_token(t_t *t, t_t **token_list)
{
	if (t->pos > ft_strlen(t->input))
		return ;
	size_t	len;
	t_t *new_token;
	int check_memory;
	int	redir_control;

	redir_control = 0;
	while (t->input[t->anchor_pos] == ' ' && t->anchor_pos < t->pos){
		t->anchor_pos++;
		}
	if(t->anchor_pos == t->pos && (t->input[t->pos] == '<' || t->input[t->pos] == '>'))
	{
		if (t->input[t->pos +1] == '<' || t->input[t->pos +1] == '>')
			return ;
		if(t->input[t->pos])
			t->pos++;
		redir_control = 1;
	}
		
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
	if (t->tmp_token)
	{
		last_str(t, ft_strdup(new_token->value), token_list);
		free(new_token->value);
		free(new_token);
		t->anchor_pos = t->pos;
		return;
	}

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
	add_token_2(new_token, token_list, redir_control, t);
}

void add_token_2(t_t *new_token, t_t **token_list, int redir_control, t_t *t)
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
	if (redir_control == 1)
		t->pos--;
}