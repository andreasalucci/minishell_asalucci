#include "../minishell.h"
#include "../libft/libft.h"

void	add_pipe(t_command **head, t_command *new_node)
{
	t_command	*tmp;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;
}

bool	check_errornclose(t_command **head, t_command *current, bool error)
{
	if (error)
	{
		free_command_list(*head);
		if (current)
			free_command(current);
	}
	return (error);
}

void	handle_redir_token(t_command **current, t_command **head,
	t_t **token)
{
	parse_commands_2(current, head, *token);
	if ((*token)->next)
		*token = (*token)->next;
}

void	handle_pipe_token(t_command **current, t_command **head, t_t *token)
{
	parse_commands_2(current, head, token);
}

void	handle_word_or_var(t_command *current, t_t *token, t_t *prev)
{
	if (!prev || !is_redir_token(prev->type))
		add_argument(current, token->value, false);
}
