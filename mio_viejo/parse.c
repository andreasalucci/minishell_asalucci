#include "minishell.h"
#include "libft.h"

void	parse(t_t *token)
{
	t_command *cmd_list;

	cmd_list = NULL;
	token->error = false;
	cmd_list = parse_commands(token);
	if (!cmd_list)
		return ;
	print_commands(cmd_list);
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

    if (!cmd)
        return;
    if (cmd->argv)
    {
        while (cmd->argv[i])
            free(cmd->argv[i++]);
        free(cmd->argv);
    }
    free(cmd->infile);
    free(cmd->outfile);
    free(cmd);
}



// funzione di prova da cancellare alla fine

void print_commands(t_command *cmd)
{
    int i = 0;
    int index = 1;

    while (cmd)
    {
        printf("🔹 Comando #%d:\n", index++);

        // Mostrar argumentos
        printf("  argv: ");
        if (cmd->argv)
        {
            while (cmd->argv[i])
            {
                printf("\"%s\" ", cmd->argv[i]);
                i++;
            }
        }
        else
            printf("(vacío)");
        printf("\n");

        // Entrada
        if (cmd->infile)
            printf("  infile: \"%s\" (modo: %d)\n", cmd->infile, cmd->redir_in);

        // Salida
        if (cmd->outfile)
            printf("  outfile: \"%s\" (modo: %d)\n", cmd->outfile, cmd->redir_out);

        printf("  ───────────────\n");

        cmd = cmd->next;
        i = 0;
    }
}
