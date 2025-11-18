#include "../minishell.h"

int	exec_and_wait(t_command *cmds, char *cmd_path, char **envp, t_env *env)
{
	int			status;
	pid_t		pid;
	struct stat	statbuf;

	pid = fork();
	if (pid == 0)
	{
		if (stat(cmd_path, &statbuf) != 0)
		{
			free_env_cmdlnull_envp(env, &cmds, false, envp);
			cmd_error_no_such_file_or_dir(cmd_path, NULL);
		}
		if (S_ISDIR(statbuf.st_mode))
		{
			free_env_cmdlnull_envp(env, &cmds, false, envp);
			cmd_error_is_a_dir(cmd_path, NULL);
		}
		if_permission_denied(cmds, cmd_path, envp, env);
		exec_and_exit(cmds, cmd_path, envp, env);
	}
	else
		parent_or_error(&pid, &status);
	free(cmd_path);
	return (0);
}

void	exec_single_non_builtin(t_command *cmds, t_env **env)
{
	char	*cmd_path;
	char	**envp;

	envp = convert_env_list_to_array(*env);
	cmd_path = get_command_path(cmds->argv[0], *env);
	if (cmd_path)
	{
		if (exec_and_wait(cmds, cmd_path, envp, *env))
		{
			free_env(*env);
			return ;
		}
	}
	else
	{
		ft_putstr_fd(cmds->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		g_exit_status = 127;
	}
	free_arrays_array(envp);
}

void	exec_single_command(t_command *cmds, t_env **env)
{
	if (is_builtin(cmds))
		exec_builtin(cmds, env);
	else
		exec_single_non_builtin(cmds, env);
}
