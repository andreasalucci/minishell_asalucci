#include "../minishell.h"

void	handle_child_cmd_path_exec_non_builtin(t_command *cmd, t_env *env,
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
		else//bastavaguardareilfile
			command_not_found(cmd, env);
	}
	else
		handle_child_cmd_path_exec_non_builtin(cmd, env, cmd_path,
			argv_filtered);
}

void	handle_child_process(t_command *cmd, t_p_fd p_fd, t_env *env)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	//signal(SIGPIPE, SIG_IGN);
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
