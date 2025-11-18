#include "../minishell.h"

char	*get_cmd_path_build_path(char ***paths, char *cmd)
{
	int		i;
	char	*candidate;

	i = 0;
	while ((*paths)[i])
	{
		candidate = ft_strjoin((*paths)[i], "/");
		candidate = ft_strjoin_free(candidate, cmd);
		if (access(candidate, X_OK) == 0)
		{
			free_arrays_array(*paths);
			return (candidate);
		}
		free(candidate);
		i++;
	}
	candidate = NULL;
	free_arrays_array(*paths);
	return (NULL);
}

char	*get_command_path(char *cmd, t_env *env)
{
	char	*path_var;
	char	**paths;

	if (its_dot_or_dotslash(cmd))
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_env_value(env, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	return (get_cmd_path_build_path(&paths, cmd));
}
