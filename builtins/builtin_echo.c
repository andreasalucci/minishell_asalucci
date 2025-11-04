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

void	init_builtin_echo_vars(int *i, int *newline, int *first)
{
	*i = 1;
	*newline = 1;
	*first = 1;
}

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	newline;
	int	first;

	init_builtin_echo_vars(&i, &newline, &first);
	while (cmd->argv[i] && is_option_n(cmd->argv[i]))
	{
		newline = 0;
		i++;
	}
	while (cmd->argv[i])
	{
		if (!cmd->arg_is_redir[i])
		{
			if (!first)
				write(1, " ", 1);
			write(1, cmd->argv[i], ft_strlen(cmd->argv[i]));
			first = 0;
		}
		i++;
	}
	if (newline)
		write(1, "\n", 1);
	g_exit_status = 0;
	return (0);
}
