#include "../minishell.h"

int	is_option_n(const char *str)
{
	int	i;

	i = 0;
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
    int i;
    int newline;
	
	i = 1;
	newline = 1;
    while (cmd->argv[i] && is_option_n(cmd->argv[i]))
    {
        newline = 0;
        i++;
    }
    while (cmd->argv[i])
    {
		if (!cmd->arg_is_redir[i])
			printf("%s", cmd->argv[i]);
		if (cmd->argv[i + 1] && !cmd->arg_is_redir[i + 1])
			printf(" ");
		i++;
    }
    if (newline)
        printf("\n");
    g_exit_status = 0;
    return (0);
}
