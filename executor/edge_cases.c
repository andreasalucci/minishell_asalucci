#include "../minishell.h"

bool	first_arg_is_one_dot(char *arg)
{
	if (ft_strcmp(arg, ".") == 0)
	{
		write(2, "minishell: ", 11);
		ft_putstr_fd(".: filename argument required\n", 2);
		g_exit_status = 2;
		return (true);
	}
	return (false);
}

bool	first_arg_is_dotslash(char *arg)
{
	if (ft_strcmp(arg, "./") == 0)
	{
		write(2, "minishell: ", 11);
		ft_putstr_fd("./: Is a directory\n", 2);
		g_exit_status = 126;
		return (true);
	}
	return (false);
}

bool	first_arg_is_all_dots(char *arg)
{
	size_t	i;
	char	*error_m;

	i = 0;
	while (arg[i] == '.')
		i++;
	if (arg[0] == '.' && arg[i] == '\0')
	{
		error_m = ft_strjoin_3("minishell: ", arg, ": command not found\n");
		ft_putstr_fd(error_m, 2);
		free(error_m);
		g_exit_status = 127;
		return (true);
	}
	return (false);
}
