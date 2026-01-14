#include "../minishell.h"

int	handle_no_args(t_env *env)
{
	t_env	*sorted;

	sorted = copy_env_list_sorted(env);
	if (!sorted)
	{
		g_exit_status = 1;
		return (1);
	}
	print_export(sorted);
	free_env(sorted);
	return (0);
}

int	is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (!str || (!ft_isalpha(str[0]) && str[0] != '_'))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_' || (str[i] == '+' && str[i
						+ 1] == '=')))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	handle_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	g_exit_status = 1;
	return (1);
}

int	process_export_arg(char *arg, t_env **env)
{
	if (!is_valid_identifier(arg))
		return (handle_invalid_identifier(arg));
	if (ft_strchr(arg, '='))
		handle_key_value_pair(arg, env);
	else
		mark_as_exportable(env, arg);
	return (0);
}

int	builtin_export(char **args, t_env **env)
{
	int	i;
	int	status;
	int	has_error;

	has_error = 0;
	i = 1;
	if (!args[1])
		return (handle_no_args(*env));
	while (args[i])
	{
		status = process_export_arg(args[i], env);
		if (status != 0)
			has_error = 1;
		i++;
	}
	if (!has_error)
		g_exit_status = 0;
	return (0);
}
