#include "../minishell.h"

char	*create_end_str(char *str1, char *str2)
{
	char	*ret;

	ret = ft_strjoin(str1, str2);
	free(str1);
	free(str2);
	return (ret);
}

void	check_pipes(t_t *t, t_t **token_list)
{
	size_t	start;
	char	*word;

	start = t->pos;
	word = NULL;
	if (t->input[t->pos] == '|')
	{
		while (start > 0 && t->input[start - 1] != ' '
			&& t->input[start - 1] != '|' && t->input[start - 1] != '<'
			&& t->input[start - 1] != '>')
			start--;
		check_pipes_2(t, token_list, start, word);
	}
	else
		add_token(t, token_list);
}

t_t	*set_metachar_type(t_t **token_list)
{
	t_t	*temp;

	temp = *token_list;
	while (temp)
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
			g_exit_status = 2;
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
			g_exit_status = 2;
			return ;
		}
	}
}

void	add_argument(t_command *cmd, char *arg, bool from_redir)
{
	int		count;
	char	**new_argv;
	bool	*new_arg_is_redir;

	new_argv = NULL;
	new_arg_is_redir = NULL;
	count = 0;
	if (cmd->argv)
		while (cmd->argv[count])
			count++;
	add_argument_alloc(cmd, &new_argv, &new_arg_is_redir, count);
	new_argv[count] = ft_strdup(arg);
	new_argv[count +1] = NULL;
	new_arg_is_redir[count] = from_redir;
	new_arg_is_redir[count + 1] = false;
	free(cmd->argv);
	free(cmd->arg_is_redir);
	cmd->argv = new_argv;
	cmd->arg_is_redir = new_arg_is_redir;
}
