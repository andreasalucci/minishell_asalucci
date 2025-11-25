#include "../minishell.h"

void	error_non_numeric(t_command *cmd)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd((cmd->argv)[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	free_command_l(cmd);
	cmd = NULL;
	exit(2);
}

void	error_too_many(t_command *cmd)
{
	ft_putstr_fd("minishell: exit: too many arguments\n", 2);
	free_command_l(cmd);
	cmd = NULL;
	exit(1);
}

void	builtin_exit(t_command *cmd)
{
	long	code;

	code = 0;
	if ((cmd->argv)[1])
	{
		if (!str_is_numeric((cmd->argv)[1]))
			error_non_numeric(cmd);
		if ((cmd->argv)[2])
			error_too_many(cmd);
		if (cmd->argv[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			g_exit_status = 1;
			free_command_l(cmd);
			cmd = NULL;
			return ;
		}
		code = ft_atolli((cmd->argv)[1]);
		free_command_l(cmd);
		cmd = NULL;
		exit((unsigned char)code);
	}
	free_command_l(cmd);
	cmd = NULL;
	exit(g_exit_status);
}
