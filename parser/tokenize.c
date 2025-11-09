#include "../minishell.h"
#include "../libft/libft.h"

void	init_struct(t_t *t)
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
	t->free_input = 0;
}

static void	handle_quotes_and_vars(t_t *t, t_t **token_list,
	bool *free_input, t_env *env)
{
	if (t->single_quote || t->double_quote)
		open_quotes(t, token_list, free_input);
	if (t->continue_var)
	{
		t->continue_var = !t->continue_var;
		return ;
	}
	if (t->input[t->pos])
	{
		if (t->input[t->pos] == '$')
		{
			is_var(t, token_list, env);
			if (t->input[t->pos] == '\'')
				return ;
		}
		metacharacters(t, token_list);
	}
}

static int	handle_token_end(t_t *t, t_t **token_list)
{
	if (!t->input[t->pos] && t->pos != t->anchor_pos)
		add_token(t, token_list);
	if (t->single_quote || t->double_quote || t->error)
		return (0);
	return (1);
}

static void	handle_syntax_error(t_t *t, t_t **token_list)
{
	if (t->single_quote || t->double_quote)
	{
		printf("minishell: syntax error near unexpected EOF\n");
		g_exit_status = 2;
	}
	if (t->tmp_token)
		free(t->tmp_token);
	free_token_list(*token_list);
	free(t->input);
}

t_t	*tokens(char *input, bool *free_input, t_env *env)
{
	t_t	t;
	t_t	*token_list;

	init_struct(&t);
	token_list = NULL;
	t.input = input;
	t.start = input;
	while (t.input[t.pos] && !t.error)
	{
		quotes(&t);
		handle_quotes_and_vars(&t, &token_list, free_input, env);
		if (!t.input[t.pos] && t.pos != t.anchor_pos)
			add_token(&t, &token_list);
	}
	if (!handle_token_end(&t, &token_list))
	{
		handle_syntax_error(&t, &token_list);
		return (NULL);
	}
	free(t.input);
	return (set_metachar_type(&token_list));
}
