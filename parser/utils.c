#include "../minishell.h"

void	handle_variable_expansion(t_t *t, size_t count)
{
	int		dollar;
	char	*var;
	char	*exp_var;

	count++;
	dollar = count;
	while (t->input[count] && (ft_isalnum(t->input[count])
			|| t->input[count] == '_'))
		count++;
	var = malloc((count - dollar) + 1);
	if (!var)
		return ;
	ft_strlcpy(var, t->input + dollar, (count - dollar) + 1);
	exp_var = getenv(var);
	if (!exp_var)
	{
		free(var);
		return ;
	}
	free(var);
	new_input(t, exp_var, count, dollar);
}

void	check_var(t_t *t)
{
	size_t	count;

	count = t->pos;
	while (count <= ft_strlen(t->input) && t->input[count] != '"')
	{
		if (t->input[count] == '$' && t->input[count + 1] == '?')
		{
			t->pos = count;
			expand_exit_status(t);
			count = t->pos;
		}
		else if (t->input[count] == '$' && t->input[count + 1] == '"')
			return ;
		else if (t->input[count] == '$' && t->input[count + 1] != ' ')
			handle_variable_expansion(t, count);
		count++;
	}
}

void	new_input(t_t *t, char *exp_var, int count, int dollar)
{
	char	*begin_var;
	char	*with_var;
	char	*new_input;
	char	*after_var;

	begin_var = malloc(count);
	after_var = malloc((ft_strlen(t->input) - count) + 1);
	ft_strlcpy(begin_var, t->input, dollar);
	with_var = ft_strjoin(begin_var, exp_var);
	ft_strlcpy(after_var, t->input + count, (ft_strlen(t->input) - count) + 1);
	new_input = ft_strjoin(with_var, after_var);
	free(begin_var);
	free(after_var);
	free(with_var);
	free(t->input);
	t->input = NULL;
	t->input = new_input;
	t->start = new_input;
}

void	handle_pipe_or_redir(t_command **current, t_command **head,
	t_t *token)
{
	if (token->type == TOKEN_REDIR_OUT)
		redir_out(*current, token);
	else if (token->type == TOKEN_DOUBLE_REDIR_OUT)
		redir_append(*current, token);
	else if (token->type == TOKEN_REDIR_IN)
		redir_in(*current, token);
	else if (token->type == TOKEN_DOUBLE_REDIR_IN)
		redir_heredoc(*current, token);
	else if (token->type == TOKEN_PIPE)
	{
		if (*current && (!(*current)->argv || !(*current)->argv[0]))
		{
			printf("minishell: syntax error near unexpected token '|'\n");
			token->error = true;
			g_exit_status = 2;
			return ;
		}
		add_pipe(head, *current);
		*current = NULL;
	}
}

void	handle_word_or_var_token(t_command **current, t_t *token)
{
	t_t	*prev;

	prev = token->prev;
	if (!prev || !is_redir_token(prev->type))
		add_argument(*current, token->value, false);
}
