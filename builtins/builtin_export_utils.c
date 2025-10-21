#include "../minishell.h"

void	init_key_value(t_key_value *data, char *arg, char *equal_pos,
		int is_append)
{
	if (is_append)
	{
		data->key = ft_substr(arg, 0, equal_pos - arg - 1);
		data->new_value_part = ft_strdup(equal_pos + 1);
	}
	else
	{
		data->key = ft_substr(arg, 0, equal_pos - arg);
		data->value = ft_strdup(equal_pos + 1);
	}
}

void	handle_append_case(t_key_value *data, t_env **env)
{
	data->old_value = get_env_value(*env, data->key);
	if (data->old_value)
		data->value = ft_strjoin(data->old_value, data->new_value_part);
	else
		data->value = ft_strdup(data->new_value_part);
	free(data->new_value_part);
}

void	update_or_add_env(t_key_value *data, t_env **env)
{
	if (env_exists(*env, data->key))
		update_env(env, data->key, data->value);
	else
		add_env(env, data->key, data->value, 1);
}

void	cleanup_key_value(t_key_value *data)
{
	free(data->key);
	free(data->value);
}
