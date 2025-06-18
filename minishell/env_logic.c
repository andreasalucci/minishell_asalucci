#include "minishell.h"

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

int	mark_as_exportable(t_env **env, char *key)
{
	t_env	*curr;

	curr = *env;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			curr->exportable = 1;
			return (1);
		}
		curr = curr->next;
	}
	return (add_env(env, key, NULL, 1));
}

int	unset_env(t_env **env, char *key)
{
	t_env *prev = NULL;
	t_env *curr = *env;

	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return (1);
		}
		prev = curr;
		curr = curr->next;
	}
	return (0);
}
