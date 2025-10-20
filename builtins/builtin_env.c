#include "../minishell.h"

void	builtin_env(t_env *env)
{
	while (env)
	{
		if (env->exportable && env->value)
		{
			write(1, env->key, ft_strlen(env->key));
			write(1, "=", 1);
			write(1, env->value, ft_strlen(env->value));
			write(1, "\n", 1);
		}
		env = env->next;
	}
}
