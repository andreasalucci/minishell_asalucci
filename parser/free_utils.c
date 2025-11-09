#include "../minishell.h"

void	free_quotes(char *str1, char *str2, char *str3)
{
	free(str1);
	free(str2);
	free(str3);
}

void	add_argument_alloc(t_command *cmd, char ***new_argv,
	bool **new_arg_is_redir, int count)
{
	int	i;

	i = 0;
	*new_argv = malloc(sizeof(char *) * (count +2));
	*new_arg_is_redir = malloc(sizeof(bool) * (count +2));
	while (i < count)
	{
		(*new_argv)[i] = cmd->argv[i];
		(*new_arg_is_redir)[i] = cmd->arg_is_redir[i];
		i++;
	}
}

void	redir_in(t_command *cmd, t_t *token)
{
	if (token->next && token->next->type == TOKEN_WORD)
	{
		add_redir(cmd, REDIR_IN, ft_strdup(token->next->value));
		token->to_remove = true;
		token->next->to_remove = true;
		token = token->next;
	}
	else
	{
		printf("minishell: syntax error near unexpected token\n");
		token->error = true;
		g_exit_status = 2;
	}
}

void	redir_out(t_command *cmd, t_t *token)
{
	if (token->next && token->next->type == TOKEN_WORD)
	{
		add_redir(cmd, REDIR_OUT, ft_strdup(token->next->value));
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
