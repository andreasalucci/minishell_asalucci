#include "../minishell.h"

int	too_many(char *oldpwd)
{
	free(oldpwd);
	ft_putstr_fd("minishell: cd: too many arguments\n", 2);
	g_exit_status = 1;
	return (1);
}

bool	blank_to_home(char **target, char **oldpwd, t_env **env)
{
	*target = get_env_value(*env, "HOME");
	if (!*target)
	{
		ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
		free(*oldpwd);
		g_exit_status = 1;
		return (false);
	}
	return (true);
}

bool	minus_to_oldpwd(char **target, char **oldpwd, t_env **env)
{
	*target = get_env_value(*env, "OLDPWD");
	if (!*target)
	{
		ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
		free(*oldpwd);
		g_exit_status = 1;
		return (false);
	}
	printf("%s\n", *target);
	return (true);
}

int	update_env_var_or_cd_error(char **oldpwd, t_env **env, char **target)
{
	char	cwd[4096];

	if (!*target || chdir(*target) != 0)
	{
		perror("cd");
		free(*oldpwd);
		g_exit_status = 1;
		return (1);
	}
	if (*oldpwd)
		update_env_var(env, "OLDPWD", *oldpwd);
	if (getcwd(cwd, 4096))
		update_env_var(env, "PWD", cwd);
	free(*oldpwd);
	g_exit_status = 0;
	return (0);
}

int	builtin_cd(char **args, t_env **env)
{
	char	*target;
	char	*oldpwd;

	oldpwd = getcwd(NULL, 0);
	if (args[1])
		if (args[2])
			return (too_many(oldpwd));
	if (args[1] && args[1][0] == '\0')
	{
		if (!blank_to_home(&target, &oldpwd, env))
			return (1);
	}
	else if (args[1] && ft_strcmp(args[1], "-") == 0)
	{
		if (!minus_to_oldpwd(&target, &oldpwd, env))
			return (1);
	}
	else if (!args[1])
	{
		if (!blank_to_home(&target, &oldpwd, env))
			return (1);
	}
	else
		target = args[1];
	return (update_env_var_or_cd_error(&oldpwd, env, &target));
}
