#include "../minishell.h"

int	is_option_n(char *str)
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

void	build_echo_arg(t_command *cmd, int i, char **result)
{
	int	first;

	first = 1;
	while (cmd->argv[i])
	{
		if (!cmd->arg_is_redir[i])
		{
			if (!first)
				*result = ft_strjoin_free(*result, " ");
			*result = ft_strjoin_free(*result, cmd->argv[i]);
			first = 0;
		}
		i++;
	}
}

int	builtin_echo(t_command *cmd)
{
	int		i;
	int		newline;
	char	*result;

	i = 1;
	newline = 1;
	while (cmd->argv[i] && is_option_n(cmd->argv[i]))
	{
		newline = 0;
		i++;
	}
	result = (char *)malloc(sizeof(char));
	result[0] = '\0';
	build_echo_arg(cmd, i, &result);
	if (newline)
		result = ft_strjoin_free(result, "\n");
	ft_putstr_fd(result, 1);
	free(result);
	g_exit_status = 0;
	return (0);
}
