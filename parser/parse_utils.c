#include "../minishell.h"
#include "../libft/libft.h"

bool	is_redir_token(int type)
{
	return (type == TOKEN_REDIR_IN
		|| type == TOKEN_DOUBLE_REDIR_IN
		|| type == TOKEN_REDIR_OUT
		|| type == TOKEN_DOUBLE_REDIR_OUT);
}

static void	handle_token(t_command **current, t_command **head, t_t **token,
	t_t *prev)
{
	if (is_redir_token((*token)->type))
		handle_redir_token(current, head, token);
	else if ((*token)->type == TOKEN_PIPE)
		handle_pipe_token(current, head, *token);
	else if ((*token)->type == TOKEN_WORD || (*token)->type == TOKEN_VAR)
		handle_word_or_var(*current, *token, prev);
}

t_command	*parse_commands(t_t *token)
{
	t_command	*head;
	t_command	*current;
	t_t			*prev;

	head = NULL;
	current = NULL;
	prev = NULL;
	while (token && !token->error)
	{
		token->prev = prev;
		prev = token;
		if (!current)
			current = init_command();
		handle_token(&current, &head, &token, prev);
		token = token->next;
	}
	if (current)
		add_pipe(&head, current);
	return (head);
}

void	parse_commands_2(t_command **current, t_command **head, t_t *token)
{
	if (token->type == TOKEN_WORD || token->type == TOKEN_VAR)
		handle_word_or_var_token(current, token);
	else
		handle_pipe_or_redir(current, head, token);
}

void	add_redir(t_command *cmd, int type, const char *filename)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->type = type;
	new->filename = (char *)filename;
	new->next = NULL;
	if (!cmd->redirs)
		cmd->redirs = new;
	else
	{
		tmp = cmd->redirs;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}
