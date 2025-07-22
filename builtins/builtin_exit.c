#include "../minishell.h"

// Funzione per controllare se una stringa è numerica
int is_numeric(const char *str)
{
	int i = 0;

	if (str[0] == '-' || str[0] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

// Funzione di supporto per convertire una stringa in long (come strtol base 10)
long mini_atoi(const char *str)
{
	long res = 0;
	int sign = 1;

	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
		res = res * 10 + (*str++ - '0');
	return (res * sign);
}

void builtin_exit(char **args)
{
	long code = 0;

	ft_putstr_fd("exit\n", 1);

	if (args[1])
	{
		if (!is_numeric(args[1]))
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
			return; // Non esce!
		}
		code = mini_atoi(args[1]);
		exit((unsigned char)code);
	}
	exit(g_exit_status);
}