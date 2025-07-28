#include "../minishell.h"

int is_option_n(const char *str)
{
    int i = 0;

    if (str[i] != '-')
        return (0);
    i++;
    while (str[i])
    {
        if (str[i] != 'n')
            return (0);
        i++;
    }
    return (1);
}

int builtin_echo(t_command *cmd)
{
    int c = 1;
	int i = 1;
    int newline = 1;
	int first = 1;

    while (cmd->argv[i] && is_option_n(cmd->argv[i]))
    {
        newline = 0;
        i++;
    }
	while (cmd->argv[c])
        c++;
    while (i < c)
    {
		if ((!(cmd->outfile) || ft_strcmp(cmd->argv[i], cmd->outfile)) &&
    		(!(cmd->infile) || ft_strcmp(cmd->argv[i], cmd->infile)))
        {
			if (!first && cmd->token_quote == 0)
				printf(" ");
			printf("%s", cmd->argv[i]);
			first = 0;
		}
        i++;
    }
	if (newline)
		printf("\n");
	g_exit_status = 0;
    return (0);
}