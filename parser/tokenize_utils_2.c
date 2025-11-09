#include "../minishell.h"
#include "../libft/libft.h"

void	check_pipes_2(t_t *t, t_t **token_list, size_t start, char *word)
{
	size_t	i;

	i = 0;
	if (start < t->pos)
	{
		word = ft_substr(t->input, start, t->pos - start);
		add_custom_token(word, TOKEN_WORD, token_list);
		free(word);
	}
	t->anchor_pos = t->pos;
	i = t->pos + 1;
	while (t->input[i] == ' ')
		i++;
	if (t->pos == 0 || t->input[i] == '\0' || t->input[i] == '|')
	{
		printf("minishell: syntax error near unexpected token '|'\n");
		t->error = true;
		g_exit_status = 2;
	}
	else
	{
		if (t->input[t->pos])
			t->pos++;
		add_token(t, token_list);
	}
}

void	add_custom_token(char *value, int type, t_t **token_list)
{
	t_t	*new_token;
	t_t	*tmp;

	tmp = NULL;
	new_token = malloc(sizeof(t_t));
	if (!new_token)
		return ;
	new_token->value = ft_strdup(value);
	new_token->type = type;
	new_token->error = false;
	new_token->next = NULL;
	if (!*token_list)
		*token_list = new_token;
	else
	{
		tmp = *token_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}

void	handle_exit_status_suffix(t_t *t, t_t **token_list, char *exit_status)
{
	size_t	start;
	char	*suffix;
	char	*joined;

	start = t->pos;
	while (t->input[t->pos] && (ft_isalnum(t->input[t->pos])
			|| t->input[t->pos] == '_'))
		t->pos++;
	suffix = ft_substr(t->input, start, t->pos - start);
	joined = ft_strjoin(exit_status, suffix);
	add_custom_token(joined, TOKEN_WORD, token_list);
	free(suffix);
	free(joined);
}

void	handle_exit_status_case(t_t *t, t_t **token_list)
{
	char	*exit_status;

	exit_status = ft_itoa(g_exit_status);
	t->pos += 2;
	if (ft_isalnum(t->input[t->pos]) || t->input[t->pos] == '_')
		handle_exit_status_suffix(t, token_list, exit_status);
	else
	{
		if (t->input[t->pos] == '\'' || t->input[t->pos] == '"')
			temp_token(t, exit_status);
		else
			add_custom_token(exit_status, TOKEN_WORD, token_list);
	}
	free(exit_status);
	t->anchor_pos = t->pos;
}

bool	handle_lonely_dollar(t_t *t, t_t **token_list)
{
	char	*tmp_str;

	if (!t->input[t->anchor_pos + 1] || t->input[t->anchor_pos + 1] == ' ')
	{
		if (t->tmp_token)
		{
			tmp_str = malloc(2);
			if (!tmp_str)
				return (true);
			tmp_str[0] = '$';
			tmp_str[1] = '\0';
			last_str(t, tmp_str, token_list);
			t->anchor_pos++;
			t->pos = t->anchor_pos;
		}
		else
		{
			add_custom_token("$", TOKEN_WORD, token_list);
			t->anchor_pos++;
			t->pos = t->anchor_pos;
		}
		return (true);
	}
	return (false);
}
