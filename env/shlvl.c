#include "../minishell.h"

void	init_shlvl(t_env **env)
{
    t_env	*tmp;
    int		shlvl;
    char	*new_val;
    int		found;

	tmp = *env;
	found = 0;
    while (tmp)
    {
        if (ft_strcmp(tmp->key, "SHLVL") == 0)
        {
            shlvl = ft_atoi(tmp->value) + 1;
            new_val = ft_itoa(shlvl);
            update_env_var(env, "SHLVL", new_val);
            free(new_val);
            found = 1;
            break;
        }
        tmp = tmp->next;
    }
    if (!found)
        add_env(env, "SHLVL", "1", 1);
}