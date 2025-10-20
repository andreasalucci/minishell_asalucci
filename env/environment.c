#include "../minishell.h"

t_env	*create_env_node(char *key, char *value, int exportable)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = ft_strdup(key);
	if (value)
		node->value = ft_strdup(value);
	else
		node->value = NULL;
	if (!node->key || (value && !node->value))
	{
		free(node->key);
		free(node->value);
		free(node);
		return (NULL);
	}
	node->exportable = exportable;
	node->next = NULL;
	return (node);
}

t_env *create_env_node_nocopy(char *key, char *value, int exportable)
{
	t_env	*node;

	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = key;      // ✅ Prende ownership direttamente
	node->value = value;  // ✅ Prende ownership direttamente
	node->exportable = exportable;
	node->next = NULL;
	return (node);
}

int	add_env(t_env **env, char *key, char *value, int exportable)
{
	t_env	*new;

	new = create_env_node(key, value, exportable);
	if (!new)
		return (0);
	new->next = *env;
	*env = new;
	return (1);
}

int	add_env_nocopy(t_env **env, char *key, char *value, int exportable)
{
	t_env	*new_node;

	new_node = create_env_node_nocopy(key, value, exportable);
	if (!new_node)
		return (0);
	new_node->next = *env;
	*env = new_node;
	return (1);
}

int	update_env(t_env **env, const char *key, const char *value)
{
	t_env	*curr;
	char	*new_val;

	if (!value)/////////////////////
		return (0);
	new_val = ft_strdup(value);
	if (!new_val)
		return (0);
	curr = *env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			free(curr->value);
			curr->value = new_val;
			return (1);
		}
		curr = curr->next;
	}
	free(new_val);
	return (0);
}

void	update_env_var(t_env **env, const char *key, const char *value)
{
	if (!env || !key)
		return ;
	if (env_exists(*env, key))
		update_env(env, key, value);
	else
		add_env(env, (char *)key, (char *)value, 1);
}

int	env_exists(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (1);
		env = env->next;
	}
	return (0);
}
