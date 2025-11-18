#include "../minishell.h"

void	command_not_found(t_command *cmd, t_env *env)
{
	if (cmd->argv)
		ft_putstr_fd(cmd->argv[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	g_exit_status = 127;
	free_env_cmdlnull_envp(env, &cmd, true, NULL);
	exit(g_exit_status);
}

bool	its_dot_or_dotslash(char *cmd)
{
	if (ft_strcmp(cmd, ".") == 0)
		return (true);
	if (ft_strcmp(cmd, "./") == 0)
		return (true);
	return (false);
}

bool	has_pipe_or_redir(t_command *cmd)
{
	return (cmd->next != NULL || cmd->redirs != NULL);
}

t_env	*copy_env(char **envp)
{
	t_env	*env;
	int		i;
	char	*eq;
	char	*key;
	char	*value;

	env = NULL;
	i = 0;
	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			key = ft_substr(envp[i], 0, eq - envp[i]);
			value = ft_strdup(eq + 1);
			add_env(&env, key, value, 1);
			free(key);
			free(value);
		}
		i++;
	}
	return (env);
}

char	**convert_env_list_to_array(t_env *env)
{
	int		count;
	t_env	*tmp;
	int		i;
	char	**envp;

	count = 0;
	tmp = env;
	i = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = malloc((count + 1) * sizeof(char *));
	tmp = env;
	while (tmp)
	{
		envp[i] = ft_strjoin_3(tmp->key, "=", tmp->value);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}
