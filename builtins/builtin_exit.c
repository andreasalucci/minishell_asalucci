#include "../minishell.h"

void	print_error_non_numeric(t_command *cmd)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((cmd->argv)[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

void	builtin_exit(t_command *cmd)
{
	long	code;

	code = 0;
	if ((cmd->argv)[1])
	{
		if (!str_is_numeric((cmd->argv)[1]))
		{
			print_error_non_numeric(cmd);
			free_command_l(cmd);
			exit(2);
		}
		if (cmd->argv[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			g_exit_status = 1;
			free_command_l(cmd);
			return ;
		}
		code = ft_atolli((cmd->argv)[1]);
		free_command_l(cmd);
		exit((unsigned char)code);
	}
	free_command_l(cmd);
	exit(g_exit_status);
}
