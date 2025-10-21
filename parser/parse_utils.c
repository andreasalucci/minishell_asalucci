#include "../minishell.h"
#include "../libft/libft.h"

bool is_redir_token(int type)
{
    return (type == TOKEN_REDIR_IN
         || type == TOKEN_DOUBLE_REDIR_IN
         || type == TOKEN_REDIR_OUT
         || type == TOKEN_DOUBLE_REDIR_OUT);
}

t_command *parse_commands(t_t *token)
{
    t_command *head = NULL;
    t_command *current = NULL;
    t_t *prev = NULL;
    while (token && !token->error)
    {
        token->prev = prev;
        prev = token;

        if (!current)
            current = init_command();

        if (is_redir_token(token->type))
        {
            parse_commands_2(&current, &head, token);
            if (token->next) // Salta il token del filename
                token = token->next;
        }
        else if (token->type == TOKEN_PIPE)
        {
            parse_commands_2(&current, &head, token);
        }
        else if (token->type == TOKEN_WORD || token->type == TOKEN_VAR)
        {
            if (!prev || !is_redir_token(prev->type))
                add_argument(current, token->value, false);
        }

        token = token->next;
    }
    if (current)
        {add_pipe(&head, current);}
	
    return head;
}

void parse_commands_2(t_command **current, t_command **head, t_t *token)
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
        if (*current && (!(*current)->argv || !(*current)->argv[0])) {
            printf("minishell: syntax error near unexpected token '|'\n");
            token->error = true;
			g_exit_status = 2;
            return;
        }
        add_pipe(head, *current);
        *current = NULL;
    }
    else if (token->type == TOKEN_WORD || token->type == TOKEN_VAR)
    {
        t_t *prev = token->prev;
        if (!prev || !is_redir_token(prev->type))
            add_argument(*current, token->value, false);
    }
}

void    add_argument_alloc(t_command *cmd, char ***new_argv, bool **new_arg_is_redir, int count)
{
    int i;
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
void	add_argument(t_command *cmd, char *arg, bool from_redir)
{

    int count;
    char **new_argv;
    bool *new_arg_is_redir;
    new_argv = NULL;
    new_arg_is_redir = NULL;
    count = 0;

		if (cmd->argv)
        	while(cmd->argv[count])
            	count++;

    add_argument_alloc(cmd, &new_argv, &new_arg_is_redir, count);
    new_argv[count] = ft_strdup(arg);
    new_argv[count +1] = NULL;  // richiesto da execve
    new_arg_is_redir[count] = from_redir;
    new_arg_is_redir[count + 1] = false;
    free(cmd->argv);
    free(cmd->arg_is_redir);
    cmd->argv = new_argv;
    cmd->arg_is_redir = new_arg_is_redir;
}

void add_redir(t_command *cmd, int type, char *filename)
{
    t_redir *new = malloc(sizeof(t_redir));
    t_redir *tmp;

    if (!new)
    {
        free(filename);
        return;
    }
    new->type = type;
    new->filename = filename;
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

void redir_out(t_command *cmd, t_t *token)
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

void redir_append(t_command *cmd, t_t *token)
{
    if (token->next && token->next->type == TOKEN_WORD)
    {
        add_redir(cmd, REDIR_APPEND, ft_strdup(token->next->value));
        token->to_remove = true;        // ">>"
        token->next->to_remove = true;  // filename
    }
    else
    {
        printf("minishell: syntax error near unexpected token\n");
        token->error = true;
		g_exit_status = 2;
    }
}

void redir_heredoc(t_command *cmd, t_t *token)
{
    if (token->next && token->next->type == TOKEN_WORD)
    {
        add_redir(cmd, REDIR_HEREDOC, ft_strdup(token->next->value));
        token->to_remove = true;        // "<<"
        token->next->to_remove = true;  // delimiter
    }
    else
    {
        printf("minishell: syntax error near unexpected token\n");
        token->error = true;
		g_exit_status = 2;
    }
}

