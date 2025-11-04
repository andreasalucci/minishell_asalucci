#include "../minishell.h"

int	handle_no_args(t_env *env)
{
	t_env	*sorted;

	sorted = copy_env_list_sorted(env);
	if (!sorted)
		return (1);
	print_export(sorted);
	free_env(sorted);
	return (0);
}

int	handle_invalid_identifier(char *arg)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	g_exit_status = 1;
	return (1);
}

void	handle_key_value_pair(char *arg, t_env **env)
{
	char		*equal_pos;
	t_key_value	data;

	equal_pos = ft_strchr(arg, '=');
	data.key = NULL;
	data.value = NULL;
	data.new_value_part = NULL;
	data.old_value = NULL;
	if (!equal_pos)
		return ;
	if (equal_pos > arg && *(equal_pos - 1) == '+')
	{
		init_key_value(&data, arg, equal_pos, 1);
		handle_append_case(&data, env);
	}
	else
		init_key_value(&data, arg, equal_pos, 0);
	update_or_add_env(&data, env);
	cleanup_key_value(&data);
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

	i = 1;
	if (!args[1])
		return (handle_no_args(*env));
	while (args[i])
	{
		process_export_arg(args[i], env);
		i++;
	}
	return (0);
}
