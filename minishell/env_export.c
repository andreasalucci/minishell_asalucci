#include "minishell.h"

t_env	*copy_env_node(t_env *src)
{
	return (create_env_node(src->key, src->value, src->exportable));
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
	while (current->next && ft_strcmp(new_node->key, current->next->key) < 0)
		current = current->next;
	new_node->next = current->next;
	current->next = new_node;
}

t_env	*sort_env_copy(t_env *env)
{
	t_env	*sorted;
	t_env	*curr;
	t_env	*copy;

	sorted = NULL;
	curr = env;
	while (curr)
	{
		copy = copy_env_node(curr);
		if (!copy)
		{
			free_env(sorted);
			return (NULL);
		}
		insert_sorted(&sorted, copy);
		curr = curr->next;
	}
	return (sorted);
}

void	builtin_export(t_env **env, char *arg)
{
	char	*key;
	char	*value;
	char	*equal_pos;

	equal_pos = ft_strchr(arg, '=');
	if (!equal_pos)
	{
		mark_as_exportable(env, arg);
		return ;
	}
	key = ft_substr(arg, 0, equal_pos - arg);
	value = ft_strdup(equal_pos + 1);
	if (env_exists(*env, key))
		update_env(env, key, value);
	else
		add_env(env, key, value, 1);
	free(key);
	free(value);
}

void	print_export(t_env *env)
{
	t_env	*sorted;

	sorted = sort_env_copy(env);
	while (sorted)
	{
		if (sorted->exportable)
		{
			if (sorted->value)
				printf("declare -x %s=\"%s\"\n", sorted->key, sorted->value);
			else
				printf("declare -x %s\n", sorted->key);
		}
		sorted = sorted->next;
	}
	free_env(sorted);
}

/*fare  unset  cd  env */
/*fare export in casi diversi*/