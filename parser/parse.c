#include "../minishell.h"
#include "../libft/libft.h"

t_command	*parse(t_t *token)
{
	t_command *cmd_list;
   
	cmd_list = NULL;
	token->error = false;
	cmd_list = parse_commands(token);
	if (!cmd_list)
		return (NULL);
	//print_commands(cmd_list);
	return (cmd_list);
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
