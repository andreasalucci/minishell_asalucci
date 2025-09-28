#include "../minishell.h"
#include "../libft/libft.h"

void	initStruct(t_t *t)
{
	t->value = NULL;
	t->type = TOKEN_DEFAULT;
	t->next = NULL;
	t->single_quote = false;
	t->double_quote = false;
	t->input = NULL;
	t->start = NULL;
	t->pos = 0;
	t->anchor_pos = 0;
	t->error = false;
	t->quote = 0;
	t->continue_var = false;
	t->tmp_token = NULL;
}

t_t	*tokens(char *input)
{
	t_t t;
	t_t *token_list;
	
	initStruct(&t);
	token_list = NULL;
	t.input = input;
	t.start = input;

	while(t.input[t.pos] && !t.error)
	{	
		quotes(&t);
		if (t.single_quote || t.double_quote)
			open_quotes(&t, &token_list);
		if (t.continue_var)
		{
			t.continue_var = !t.continue_var;
			continue;
		}
		if(t.input[t.pos])
		{
			if (t.input[t.pos] == '$')
				is_var(&t, &token_list);
			
			//ft_printf("t->pos antes metacharacters:: %c\n\n", t.input[t.pos]);
			metacharacters(&t, &token_list);
			//ft_printf("t->pos depues metacharacters:: %c\n\n", t.input[t.pos]);
		}
		if (!t.input[t.pos] && t.pos != t.anchor_pos)
			add_token(&t, &token_list);
		//ft_printf("final\nt->pos:: %i\n\n", t.pos);
	}
	if (t.single_quote || t.double_quote || t.error)
	{
		if (t.single_quote || t.double_quote)
			printf("minishell: syntax error near unexpected EOF\n");
		return (0);
	}

	 return (set_metachar_type(&token_list));
}
t_t	*set_metachar_type(t_t **token_list)
{
	t_t *temp;

	temp = *token_list;
	while(temp)
	{
		if (temp->type == METACHAR)
		{
			if (ft_strncmp(temp->value, "|", 1) == 0)
				temp->type = TOKEN_PIPE;
			else if (ft_strncmp(temp->value, "<<", 3) == 0)
			{
				temp->type = TOKEN_DOUBLE_REDIR_IN;
			}
				
			else if (ft_strncmp(temp->value, "<", 1) == 0)
			{
				temp->type = TOKEN_REDIR_IN;
			}
				
			else if (ft_strncmp(temp->value, ">>", 3) == 0)
				temp->type = TOKEN_DOUBLE_REDIR_OUT;
			else if (ft_strncmp(temp->value, ">", 1) == 0)
				temp->type = TOKEN_REDIR_OUT;
			
		}
		temp = temp->next;
	}
	return (*token_list);
}
void	triple_meta(t_t *t, t_t **token_list)
{
	if (t->input[t->pos] == '<')
	{
		if (t->input[t->pos + 1] == '<' && t->input[t->pos + 2] == '<')
		{
			printf("minishell: syntax error near unexpected token\n");
			t->pos += 2;
			t->error = true;
			add_token(t, token_list);
			return ;
		}
	}
	 
	else if (t->input[t->pos] == '>')
	{
		if (t->input[t->pos + 1] == '>' && t->input[t->pos + 2] == '>')
		{
			printf("minishell: syntax error near unexpected token\n");
			t->pos += 2;
			t->error = true;
			add_token(t, token_list);
			return ;
		}
	}
}

void    check_pipes(t_t *t, t_t **token_list)
{
    size_t  start;
    char    *word;

    start = t->pos;
    word = NULL;
    if (t->input[t->pos] == '|')
    {

        while (start > 0 && t->input[start - 1] != ' ' &&
               t->input[start - 1] != '|' && t->input[start - 1] != '<' &&
               t->input[start - 1] != '>')
            start--;
		check_pipes_2(t, token_list, start, word);
    }
    else
        add_token(t, token_list);
	
}



