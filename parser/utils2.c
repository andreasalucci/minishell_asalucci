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
		printf("minishell: syntax error near unexpected token\n");
		token->error = true;
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
		printf("minishell: syntax error near unexpected token\n");
		token->error = true;
		g_exit_status = 2;
	}
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
	if (t->tmp_token)
		last_str(t, var_word, token_list);
	else
	{
		add_custom_token(var_word, TOKEN_VAR, token_list);
		free(var_word);
	}
	free(var_temp);
	t->anchor_pos = t->pos;
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
	if (t->input[t->pos] == '$' && t->input[t->pos + 1] == '?')
	{
		handle_exit_status_case(t, token_list);
		return ;
	}
	if (expand_exit_status(t))
		return ;
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
