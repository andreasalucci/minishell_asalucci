#include "../minishell.h"

static void	handle_var_result(t_t *t, t_t **token_list,
			char *prefix, char *var)
{
	char	*end_var;
	char	*var_token;

	var_token = getenv(var);
	if (!var_token)
	{
		end_var = ft_strjoin(prefix, var);
		add_custom_token(end_var, TOKEN_WORD, token_list);
		free(end_var);
		free(prefix);
		free(var);
		t->pos++;
		t->anchor_pos = t->pos;
		return ;
	}
	end_var = ft_strjoin(prefix, var_token);
	add_custom_token(end_var, TOKEN_VAR, token_list);
	free(end_var);
	free(prefix);
	free(var);
	t->anchor_pos = t->pos;
}

void	is_var_2(t_t *t, t_t **token_list)
{
	size_t	dolar;
	size_t	len;
	char	*prefix;
	char	*var;

	dolar = t->pos;
	prefix = malloc(t->pos - t->anchor_pos + 1);
	if (!prefix)
		return ;
	ft_strlcpy(prefix, t->input + t->anchor_pos, (t->pos - t->anchor_pos) + 1);
	while (t->input[t->pos + 1] && (ft_isalnum(t->input[t->pos + 1])
			|| t->input[t->pos + 1] == '_'))
		t->pos++;
	if (t->pos != dolar)
		dolar++;
	len = t->pos - dolar;
	var = malloc(len + 2);
	if (!var)
	{
		free(prefix);
		return ;
	}
	ft_strlcpy(var, t->input + dolar, len + 2);
	handle_var_result(t, token_list, prefix, var);
}

void	handle_double_quote(t_t *t, t_t **token_list, bool *free_input)
{
	t->double_quote = !t->double_quote;
	if (t->pos > t->anchor_pos)
	{
		if (t->quote != t->anchor_pos)
			prepare_quotes(t, token_list, free_input);
		else
		{
			if (t->input[t->anchor_pos] == '\"')
				t->anchor_pos++;
			prepare_str(t, token_list);
			if (t->input[t->pos])
				t->pos++;
			t->anchor_pos = t->pos;
		}
	}
	if (t->input[t->pos] == '\"')
		t->continue_var = !t->continue_var;
}

void	handle_single_quote(t_t *t, t_t **token_list, bool *free_input)
{
	t->single_quote = !t->single_quote;
	if (t->pos > t->anchor_pos)
	{
		if (t->quote != t->anchor_pos)
			prepare_quotes(t, token_list, free_input);
		else
		{
			if (t->input[t->anchor_pos] == '\'')
				t->anchor_pos++;
			prepare_str(t, token_list);
			t->anchor_pos = t->pos + 1;
		}
	}
	if (t->input[t->pos])
		t->pos++;
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
