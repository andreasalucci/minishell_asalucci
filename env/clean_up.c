#include "../minishell.h"

void	free_env(t_env *env)
{
	t_env	*tmp;

	//printf("DEBUG FREE ENV VIENE actually FATTO WWWWWWWWWWWWWWWWWWWWWWW\n\n");
	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
	env = NULL;
}
