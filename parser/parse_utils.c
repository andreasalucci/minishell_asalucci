#include "../minishell.h"
#include "../libft/libft.h"

t_command	*parse_commands(t_t *token)
{
	t_command *head;
	t_command *current;
	t_t *prev_token;
	
	prev_token = NULL;
	head = NULL;
	current = NULL;
	while(token && !token->error)
	{
		
		if (!current)
		{
			current = malloc(sizeof(t_command));
			ft_bzero(current, sizeof(t_command));
		}
		if ((token->type == TOKEN_WORD || token->type == TOKEN_VAR) &&
        (!prev_token || prev_token->type != TOKEN_REDIR_IN))
		{
			add_argument(current, token->value, token->token_quote);
		}
		
		else
			parse_commands_2(&current, &head, token);
		if (check_errorNclose(&head, current, token->error))
			return (NULL);
		prev_token = token;
		token = token->next;
	}
	if (current && (!token || !token->error))
		add_pipe(&head, current);
	return (head);
}

void	parse_commands_2(t_command **current, t_command **head, t_t *token)
{
	if (token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_DOUBLE_REDIR_OUT)
			redir_out(*current, token);
	else if (token->type == TOKEN_REDIR_IN
		|| token->type == TOKEN_DOUBLE_REDIR_IN)
			redir_in(*current, token);
	else if (token->type == TOKEN_PIPE)
	{
		add_pipe(head, *current);
		*current = NULL;
	}
	
}

void	add_argument(t_command *cmd, char *arg, int token_quote)
{
	int	count;
	char **new_argv;
	int	i;

	i = 0;
	new_argv = NULL;
	count = 0;
	if (cmd->argv)
		while(cmd->argv[count])
			count++;
	new_argv = malloc(sizeof(char *) * (count +2));
	while (i < count)
	{
		new_argv[i] = cmd->argv[i];
		i++;
	}
	new_argv[count] = ft_strdup(arg);
	new_argv[count +1] = NULL;	// richiesto da execve
	free(cmd->argv);
	cmd->argv = new_argv;
	if (token_quote == 1)
		cmd->token_quote = 1;
}


void add_redir(t_command *cmd, int type, const char *filename)
{
    t_redir *new = malloc(sizeof(t_redir));
    t_redir *tmp;

    if (!new)
        return; // gestisci errore malloc
    new->type = type;
    new->filename = ft_strdup(filename);
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

void redir_in(t_command *cmd, t_t *token)
{
    if (token->next && token->next->type == TOKEN_WORD)
    {
        add_redir(cmd, REDIR_IN, ft_strdup(token->next->value));

        // Escludi dal vettore argv
        token->to_remove = true;        // "<"
        token->next->to_remove = true;  // filename
    }
    else
    {
        ft_printf("minishell: syntax error near unexpected token\n");
        token->error = true;
    }
}

void redir_out(t_command *cmd, t_t *token)
{
    if (token->next && token->next->type == TOKEN_WORD)
    {
        add_redir(cmd, REDIR_OUT, ft_strdup(token->next->value));

        // Escludi dal vettore argv
        token->to_remove = true;        // ">"
        token->next->to_remove = true;  // filename
    }
    else
    {
        ft_printf("minishell: syntax error near unexpected token\n");
        token->error = true;
    }
}

void redir_append(t_command *cmd, t_t *token)
{
    if (token->next && token->next->type == TOKEN_WORD)
    {
        add_redir(cmd, REDIR_APPEND, ft_strdup(token->next->value));

        // Escludi dal vettore argv
        token->to_remove = true;        // ">>"
        token->next->to_remove = true;  // filename
    }
    else
    {
        ft_printf("minishell: syntax error near unexpected token\n");
        token->error = true;
    }
}

void redir_heredoc(t_command *cmd, t_t *token)
{
    if (token->next && token->next->type == TOKEN_WORD)
    {
        add_redir(cmd, REDIR_HEREDOC, ft_strdup(token->next->value));

        // Escludi dal vettore argv
        token->to_remove = true;        // "<<"
        token->next->to_remove = true;  // delimiter
    }
    else
    {
        ft_printf("minishell: syntax error near unexpected token\n");
        token->error = true;
    }
}

