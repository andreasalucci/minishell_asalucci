#include "../minishell.h"
#include "../libft/libft.h"

void	parse(t_t *token)
{
	t_command *cmd_list;
   
	cmd_list = NULL;
	token->error = false;
     
	cmd_list = parse_commands(token);
	if (!cmd_list)
		return ;
	//print_commands(cmd_list);
}

void free_command_list(t_command *cmd)
{
    t_command *tmp;
    int i;

	i = 0;
    while (cmd)
    {
        tmp = cmd->next;
        if (cmd->argv)
        {
            while (cmd->argv[i])
                free(cmd->argv[i++]);
            free(cmd->argv);
        }
        free(cmd->infile);
        free(cmd->outfile);
        free(cmd);
        cmd = tmp;
    }
}

void free_token_list(t_t *token)
{
    t_t *tmp;

    while (token)
    {
        tmp = token->next;
        free(token->value);
        free(token);
        token = tmp;
    }
}

void free_command(t_command *cmd)
{
    int i = 0;
    t_redir *r;
	t_redir *tmp;

    if (!cmd)
		return;
    if (cmd->argv)
    {
        while (cmd->argv[i])
            free(cmd->argv[i++]);
        free(cmd->argv);
    }
    r = cmd->redirs;
    while (r)
    {
        tmp = r->next;
        free(r->filename);
        free(r);
        r = tmp;
    }
    free(cmd->infile);
    free(cmd->outfile);
    free(cmd);
}



// funzione di prova da cancellare alla fine

void print_commands(t_command *cmd)
{
    int i;
    int index = 1;
    t_redir *r;

    while (cmd)
    {
        printf("🔹 Comando #%d:\n", index++);

        // Argomenti
        printf("  argv: ");
        if (cmd->argv)
        {
            i = 0;
            while (cmd->argv[i])
            {
                printf("\"%s\" ", cmd->argv[i]);
                i++;
            }
        }
        else
            printf("(vuoto)");
        printf("\n");

        // Redirezioni
        if (cmd->redirs)
        {
            printf("  redirs:\n");
            r = cmd->redirs;
            while (r)
            {
                if (r->type == REDIR_IN)
                    printf("    <  \"%s\"\n", r->filename);
                else if (r->type == REDIR_OUT)
                    printf("    >  \"%s\"\n", r->filename);
                else if (r->type == REDIR_APPEND)
                    printf("    >> \"%s\"\n", r->filename);
                else if (r->type == REDIR_HEREDOC)
                    printf("    << \"%s\"\n", r->filename);
                r = r->next;
            }
        }
        else
            printf("  redirs: (nessuna)\n");

        printf("  ───────────────\n");

        cmd = cmd->next;
    }
}
