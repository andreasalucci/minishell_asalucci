#include "../minishell.h"

void	if_permission_denied(t_command *cmds, char *cmd_path, char **envp,
		t_env *env)
{
	if (access(cmd_path, X_OK) != 0)
	{
		free_env_cmdlnull_envp(env, &cmds, false, envp);
		cmd_error_permission_denied(cmd_path, NULL);
	}
}

void	execute_and_exit(t_command *cmds, char *cmd_path, char **envp, t_env *env)
{
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	execve(cmd_path, cmds->argv, envp);
	perror(cmd_path);
	free(cmd_path);
	free_env_cmdlnull_envp(env, &cmds, false, envp);
	exit(1);
}

void	parent_or_error(pid_t *pid, int *status)
{
	int	signal;

	if (*pid > 0)
	{
		waitpid(*pid, status, 0);
		if (WIFEXITED(*status))
			g_exit_status = WEXITSTATUS(*status);
		else if (WIFSIGNALED(*status))
		{
			signal = WTERMSIG(*status);
			g_exit_status = 128 + signal;
			if (signal == SIGQUIT && isatty(STDOUT_FILENO))
				ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
		}
	}
	else
		perror("fork");
}
