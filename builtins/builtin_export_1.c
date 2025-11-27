#include "../minishell.h"

void	cleanup_key_value(t_key_value *data)
{
	free(data->key);
	free(data->value);
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

void	print_export(t_env *sorted)
{
	while (sorted)
	{
		write(1, "declare -x ", 11);
		write(1, sorted->key, ft_strlen(sorted->key));
		if (sorted->value)
		{
			write(1, "=\"", 2);
			write(1, sorted->value, ft_strlen(sorted->value));
			write(1, "\"", 1);
		}
		write(1, "\n", 1);
		sorted = sorted->next;
	}
}

void	insert_sorted(t_env **sorted, t_env *new_node)
{
	t_env	*current;

	if (!*sorted || ft_strcmp(new_node->key, (*sorted)->key) < 0)
	{
		new_node->next = *sorted;
		*sorted = new_node;
		return ;
	}
	current = *sorted;
	while (current->next && ft_strcmp(new_node->key, current->next->key) > 0)
		current = current->next;
	new_node->next = current->next;
	current->next = new_node;
}

t_env	*copy_env_list_sorted(t_env *env)
{
	t_env	*sorted;
	t_env	*new;

	sorted = NULL;
	while (env)
	{
		if (env->exportable)
		{
			new = malloc(sizeof(t_env));
			if (!new)
				return (NULL);
			new->key = ft_strdup(env->key);
			if (env->value)
				new->value = ft_strdup(env->value);
			else
				new->value = NULL;
			new->exportable = 1;
			new->next = NULL;
			insert_sorted(&sorted, new);
		}
		env = env->next;
	}
	return (sorted);
}
