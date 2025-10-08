#include "../minishell.h"

int	too_many(void)
{
	ft_putstr_fd(" too many arguments\n", 2);
	g_exit_status = 1;
	return (1);
}

int	builtin_cd(char **args, t_env **env)
{
	char	*target;
	char	*oldpwd;
	char	cwd[4096];

	oldpwd = getcwd(NULL, 0);
	if (args[2])
	{
		free(oldpwd);
		return (too_many());
	}
		
	if (!args[1])
		target = get_env_value(*env, "HOME");
	else
		target = args[1];
	if (!target || chdir(target) != 0)
	{
		perror("cd");
		free(oldpwd);
		g_exit_status = 1;
		return (1);
	}
	if (oldpwd)
		update_env_var(env, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		update_env_var(env, "PWD", cwd);
	free(oldpwd);
	g_exit_status = 0;
	return (0);
}
