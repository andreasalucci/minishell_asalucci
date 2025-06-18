#include "minishell.h"

int	main(void)
{
	t_env *env = NULL;

	add_env(&env, "USER", "marvin", 1);
	add_env(&env, "PATH", "/bin", 1);
	add_env(&env, "SECRET", "42", 0);

	// Prova a "esportare" SECRET
	mark_as_exportable(&env, "SECRET");
	// Aggiungi variabile senza valore ma esportabile
	mark_as_exportable(&env, "NO_VALUE");

	t_env *sorted = sort_env_copy(env);
	t_env *tmp = sorted;

	printf("🧪 Simulazione export:\n\n");
	while (tmp)
	{
		if (tmp->exportable)
		{
			if (tmp->value)
				printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
			else
				printf("declare -x %s\n", tmp->key);
		}
		tmp = tmp->next;
	}

	// Libera tutto
	free_env(sorted);
	free_env(env);
	return 0;
}