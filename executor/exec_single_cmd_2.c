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

void	exec_and_exit(t_command *cmds, char *cmd_path, char **envp, t_env *env)
{
	execve(cmd_path, cmds->argv, envp);
	perror(cmd_path);
	free(cmd_path);
	free_env_cmdlnull_envp(env, &cmds, false, envp);
	exit(1);
}

void	parent_or_error(pid_t *pid, int *status)
{
	if (*pid > 0)
	{
		waitpid(*pid, status, 0);
		if (WIFEXITED(*status))
			g_exit_status = WEXITSTATUS(*status);
		else if (WIFSIGNALED(*status))
			g_exit_status = 128 + WTERMSIG(*status);
	}
	else
		perror("fork");
}
