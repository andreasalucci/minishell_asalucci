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
	return (1);
}

void	handle_key_value_pair(char *arg, t_env **env)
{
	char	*equal_pos;
	char	*key;
	char	*value;
	char	*old_value;
	char	*new_value_part;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
		return;
	if (equal_pos > arg && *(equal_pos - 1) == '+')
	{
		equal_pos--;
		key = ft_substr(arg, 0, equal_pos - arg);
		new_value_part = ft_strdup(equal_pos + 2);
		old_value = get_env_value(*env, key);
		if (old_value)
			value = ft_strjoin(old_value, new_value_part);
		else
			value = ft_strdup(new_value_part);
		if (env_exists(*env, key))
			update_env(env, key, value);
		else
			add_env(env, key, value, 1);
		free(new_value_part);
	}
	else
	{
		key = ft_substr(arg, 0, equal_pos - arg);
		value = ft_strdup(equal_pos + 1);
		if (env_exists(*env, key))
			update_env(env, key, value);
		else
			add_env(env, key, value, 1);
	}
	free(key);
	free(value);
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
