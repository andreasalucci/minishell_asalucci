#include "../minishell.h"

void	execute_child_cmd_path(t_command *cmd, t_env *env,
					char *cmd_path, char **argv_filtered)
{
	struct stat	statbuf;

	if (stat(cmd_path, &statbuf) != 0)
		cmd_error_no_such_file_or_dir(cmd_path, argv_filtered);
	if (S_ISDIR(statbuf.st_mode))
		cmd_error_is_a_dir(cmd_path, argv_filtered);
	if (access(cmd_path, X_OK) != 0)
		cmd_error_permission_denied(cmd_path, argv_filtered);
	filter_args(cmd, &argv_filtered, env);
	execve(cmd_path, argv_filtered, convert_env_list_to_array(env));
	perror(cmd_path);
	free(argv_filtered);
	free(cmd_path);
	exit(1);
}

void	handle_child_cmd_path(t_command *cmd, t_env *env)
{
	char	*cmd_path;
	char	**argv_filtered;

	argv_filtered = NULL;
	if (cmd->argv && is_builtin(cmd))
	{
		signal(SIGPIPE, SIG_IGN);
		exec_builtin(cmd, &env);
		free_env_cmdlnull_envp(env, &cmd, true, NULL);
		exit(g_exit_status);
	}
	if (cmd->argv)
		cmd_path = get_command_path(cmd->argv[0], env);
	else
		cmd_path = NULL;
	if (!cmd_path)
	{
		if (!cmd->argv && cmd->in_hdc)
			no_command_heredoc(cmd, env);
		else
			command_not_found(cmd, env);
	}
	else
		execute_child_cmd_path(cmd, env, cmd_path,
			argv_filtered);
}

void	handle_child_process(t_command *cmd, t_p_fd p_fd, t_env *env)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (p_fd.prev_fd != -1)
	{
		dup2(p_fd.prev_fd, STDIN_FILENO);
		close(p_fd.prev_fd);
	}
	if (cmd->next)
	{
		close(p_fd.pipe_fd[0]);
		dup2(p_fd.pipe_fd[1], STDOUT_FILENO);
		close(p_fd.pipe_fd[1]);
	}
	apply_redirections(cmd, env);
	handle_child_cmd_path(cmd, env);
}

void	child_and_parent_process(pid_t pid, t_command **cmd, t_p_fd *p_fd,
									t_env *env)
{
	t_command	*tmp;

	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		handle_child_process(*cmd, *p_fd, env);
		free_env_cmdlnull_envp(env, cmd, true, NULL);
		exit(1);
	}
	else
	{
		if ((*p_fd).prev_fd != -1)
			close((*p_fd).prev_fd);
		if ((*p_fd).pipe_fd[1] != -1)
		{
			close((*p_fd).pipe_fd[1]);
			(*p_fd).prev_fd = (*p_fd).pipe_fd[0];
		}
		else if ((*p_fd).pipe_fd[0] != -1)
			close((*p_fd).pipe_fd[0]);
		tmp = (*cmd)->next;
		free_command(*cmd);
		*cmd = tmp;
	}
}

void	wait_for_children(pid_t last_pid)
{
	int		status;
	pid_t	pid;
	int		sig;

	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				sig = WTERMSIG(status);
				g_exit_status = 128 + sig;
				if (sig == SIGQUIT && isatty(STDOUT_FILENO))
					ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
			}
			else
				g_exit_status = 1;
		}
		pid = wait(&status);
	}
	if (pid == -1 && errno != ECHILD)
		perror("waitpid");
}
