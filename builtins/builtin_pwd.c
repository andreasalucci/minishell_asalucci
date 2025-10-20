#include "../minishell.h"

int	builtin_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		g_exit_status = 1;
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	g_exit_status = 0;
	return (0);
}
