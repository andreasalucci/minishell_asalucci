#include "../minishell.h"

void	redir_append(t_command *cmd, t_t *token)
{
	if (token->next && token->next->type == TOKEN_WORD)
	{
		add_redir(cmd, REDIR_APPEND, ft_strdup(token->next->value));
		token->to_remove = true;
		token->next->to_remove = true;
	}
	else
	{
		printf("minishell: syntax error near unexpected token `>>'\n");
		token->error = true;
		cmd->contrasting_redirs = true;
		g_exit_status = 2;
	}
}

void	redir_heredoc(t_command *cmd, t_t *token)
{
	if (token->next && token->next->type == TOKEN_WORD)
	{
		add_redir(cmd, REDIR_HEREDOC, ft_strdup(token->next->value));
		token->to_remove = true;
		token->next->to_remove = true;
	}
	else
	{
		printf("minishell: syntax error near unexpected token `<<'\n");
		token->error = true;
		cmd->contrasting_redirs = true;
		g_exit_status = 2;
	}
}

void	new_input_2(t_t *t, char *var_word, size_t count, size_t dollar)
{
	char	*before_var;
	char	*afer_var;
	char	*with_var;
	char	*new_input;

	before_var = malloc(dollar + 1);
	afer_var = malloc((ft_strlen(t->input) + 1) - dollar);
	ft_strlcpy(before_var, t->input, dollar +1);
	with_var = ft_strjoin(before_var, var_word);
	ft_strlcpy(afer_var, t->input + count, ft_strlen(t->input) + 1 - count);
	new_input = ft_strjoin(with_var, afer_var);
	free(before_var);
	free(afer_var);
	free(with_var);
	free(var_word);
	free(t->input);
	t->input = NULL;
	t->input = new_input;
	t->start = new_input;
}

void	finalize_env_var(t_t *t, t_t **token_list, char *var, char *var_temp)
{
	char	*var_word;

	if (!var)
	{
		free(var_temp);
		t->anchor_pos = t->pos;
		if (t->tmp_token)
		{
			add_custom_token(t->tmp_token, TOKEN_WORD, token_list);
			free(t->tmp_token);
			t->tmp_token = NULL;
		}
		return ;
	}
	var_word = ft_strdup(var);
	new_input_2(t, var_word, t->pos, t->anchor_pos);
	free(var_temp);
	t->pos = t->anchor_pos;
}

void	handle_env_var(t_t *t, t_t **token_list, t_env *env)
{
	char	*var_temp;
	char	*var;
	size_t	len;

	if (t->pos == t->anchor_pos)
		t->pos++;
	while (t->input[t->pos] && (ft_isalnum(t->input[t->pos])
			|| t->input[t->pos] == '_'))
		t->pos++;
	len = t->pos - (t->anchor_pos + 1);
	var_temp = malloc(len + 1);
	if (!var_temp)
		return ;
	ft_strlcpy(var_temp, t->input + t->anchor_pos + 1, len + 1);
	var = get_env_value(env, var_temp);
	finalize_env_var(t, token_list, var, var_temp);
}

void	is_var(t_t *t, t_t **token_list, t_env *env)
{
	if (t->single_quote)
		return ;
	if (t->input[t->pos] == '$' && t->input[t->pos + 1] == '?')
	{
		if (expand_exit_status(t))
			return ;
	}
	if (t->input[t->anchor_pos] == ' ')
		t->anchor_pos++;
	if (t->input[t->anchor_pos] == '$')
	{
		if (handle_lonely_dollar(t, token_list))
			return ;
		handle_env_var(t, token_list, env);
	}
	else
		is_var_2(t, token_list);
}
