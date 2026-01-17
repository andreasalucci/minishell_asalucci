#include "../minishell.h"

void	new_input_3(t_t *t, char *end_var)
{
	char	*begin_var;
	char	*after_var;
	char	*with_var;
	char	*new_input;

	begin_var = malloc(t->anchor_pos +1);
	after_var = malloc((strlen(t->input) - t->pos) + 1);
	ft_strlcpy(begin_var, t->input, t->anchor_pos +1);
	ft_strlcpy(after_var, t->input + (t->pos +1), strlen(t->input) - t->pos);
	with_var = ft_strjoin(begin_var, end_var);
	new_input = ft_strjoin(with_var, after_var);
	free(with_var);
	free(after_var);
	free(begin_var);
	free(t->input);
	t->input = NULL;
	t->input = new_input;
	t->start = new_input;
}

char*	ft_after_var(t_t *t, char *var)
{
	size_t	len;
	char	*after_var;

	len = strlen(var) + t->anchor_pos +1;
	after_var = malloc((t->pos - len) +1);
	ft_strlcpy(after_var, t->input + len, (t->pos - len) +1);
	return (after_var);
}
void	handle_var_result(t_t *t, t_t **token_list,
			char *prefix, char *var)
{
	char	*end_var;
	char	*var_token;
	char	*after_var;

	after_var = NULL;
	end_var = NULL;
	var_token = getenv(var);
	if (!var_token)
	{
		if (t->num_var == true)
			after_var = ft_after_var(t, var);
		t->num_var = false;
		if (after_var)
		{
			end_var = ft_strjoin(prefix, after_var);
			add_custom_token(end_var, TOKEN_WORD, token_list);
			free(after_var);
		}
		else
			add_custom_token(prefix, TOKEN_WORD, token_list);
		free(prefix);
		free(var);
		t->pos++;
		t->anchor_pos = t->pos;
		return ;
	}
	end_var = ft_strjoin(prefix, var_token);
	new_input_3(t, end_var);
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
	if (t->input[t->pos + 1] >= '0' && t->input[t->pos + 1] <= '9')
		t->num_var = true;
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
