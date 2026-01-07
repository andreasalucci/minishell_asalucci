#include "../minishell.h"

int	too_many(char *oldpwd)
{
	free(oldpwd);
	ft_putstr_fd("minishell: cd: too many arguments\n", 2);
	g_exit_status = 1;
	return (1);
}

int	builtin_cd(char **args, t_env **env)
{
	char	*target;
	char	*oldpwd;
	char	cwd[4096];

	oldpwd = getcwd(NULL, 0);
	if (args[1])
		if (args[2])
			return (too_many(oldpwd));

	// if (!args[1])
	// 	printf("non esiste\n");
	// if (args[1])
	// 	printf("<%s>\n", args[1]);
	// if (args[1] && args[1][0] == '\0')
	// 	printf("e' un null character\n");
	// if (args[1])
	// 	printf("args[1] ritorna true e <%ld> e' la lunghezza\n", strlen(args[1]));
	// printf("args[1] ritorna false e <%ld> e' la lunghezza\n", strlen(args[1]));

	if (args[1] && args[1][0] == '\0')
	{
		target = get_env_value(*env, "HOME");
		if (!target)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			free(oldpwd);
			g_exit_status = 1;
			return (1);
		}
	}
	else if (args[1] && ft_strcmp(args[1], "-") == 0)
	{
		target = get_env_value(*env, "OLDPWD");
		if (!target)
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", STDERR_FILENO);
			free(oldpwd);
			g_exit_status = 1;
			return (1);
		}
		printf("%s\n",target);
	}
	else if (!args[1])
	{
		target = get_env_value(*env, "HOME");
		if (!target)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", STDERR_FILENO);
			free(oldpwd);
			g_exit_status = 1;
			return (1);
		}
	}
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
