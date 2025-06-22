#include "../minishell.h"

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
