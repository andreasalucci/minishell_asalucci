#include "../minishell.h"

void	builtin_exit(char **args)
{
	long	code;

	code = 0;
	ft_putstr_fd("exit\n", 1);
	if (args[1])
	{
		if (!str_is_numeric(args[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			exit(2);
		}
		if (args[2])
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
			g_exit_status = 1;
			return ;
		}
		code = ft_atolli(args[1]);
		exit((unsigned char)code);
	}
	exit(g_exit_status);
}
