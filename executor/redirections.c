#include "../libft/libft.h"
#include "../minishell.h"

static void	heredoc_sigint_handler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	g_exit_status = 130;
	close(STDIN_FILENO);
}

void apply_redirections(t_command *cmd, t_env *env)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_IN)
			apply_redir_in1(r, env, cmd);
		else if (r->type == REDIR_OUT)
			apply_redir_out1(r, env, cmd);
		else if (r->type == REDIR_APPEND)
			apply_redir_out2(r, cmd, env);
		else if (r->type == REDIR_HEREDOC)
			apply_redir_heredoc(cmd, env);
		r = r->next;
	}
}

void	apply_redir_in1(t_redir *r, t_env *env, t_command *cmd)
{
	int	fd;

	fd = open(r->filename, O_RDONLY);
	if (fd < 0)
	{
		if (errno == ENOENT)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(r->filename, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			g_exit_status = 1;
		}
		else if (errno == ENOTDIR)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(r->filename, 2);
			ft_putstr_fd(": Not a directory\n", 2);
			g_exit_status = 1;
		}
		else
		{
			perror(r->filename);
			g_exit_status = 1;
		}
		free_env_cmdl_null(env, &cmd);
		exit(g_exit_status);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	apply_redir_out1(t_redir *r, t_env *env, t_command *cmd)
{
	int	fd;

	fd = open(r->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		if (errno == ENOTDIR)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(r->filename, 2);
			ft_putstr_fd(": Not a directory\n", 2);
			g_exit_status = 1;
		}
		else
		{
			perror(r->filename);
			g_exit_status = 1;
		}
		free_env_cmdl_null(env, &cmd);
		exit(g_exit_status);
	}
	if (r->next && (r->next->type == REDIR_OUT || r->next->type == REDIR_APPEND))
		close(fd);
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void	apply_redir_out2(t_redir *r, t_command *cmd, t_env *env)
{
	int	fd;

	fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		if (errno == ENOTDIR)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(r->filename, 2);
			ft_putstr_fd(": Not a directory\n", 2);
			g_exit_status = 1;
		}
		else
		{
			perror(r->filename);
			g_exit_status = 1;
		}
		free_env_cmdl_null(env, &cmd);
		exit(g_exit_status);
	}
	if (r->next && (r->next->type == REDIR_OUT || r->next->type == REDIR_APPEND))
		close(fd);
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void apply_redir_heredoc(t_command *cmd, t_env *env)
{
	int fd;

	fd = open(".heredoc_tmp", O_RDONLY);
	if (fd < 0)
	{
		perror(".heredoc_tmp");
		free_env_cmdl_null(env, &cmd);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	create_heredoc_effective(const char *delimiter, t_command *cmd, t_env *env)
{
	int		fd;
	char	*line;

	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open .heredoc_tmp");
		free_env_cmdl_null(env, &cmd);
		exit(1);
	}
	signal(SIGINT, heredoc_sigint_handler);
	while (1)
	{
		line = readline("> ");
		if (g_exit_status == 130)
		{
			free_env_cmdl_null(env, &cmd);
			exit(130);
		}
		if (!line)
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_clear_history();
			break;
		}
		if (*line)
			add_history(line);

		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		fsync(fd);
		free(line);
	}
	close(fd);
	free_env_cmdl_null(env, &cmd);
	exit(0);
}

void	heredoc_open_interrupted(int status, bool *hrd_interrupted)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		*hrd_interrupted = true;
		g_exit_status = 130;
		unlink(".heredoc_tmp");
	}
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		*hrd_interrupted = true;
		g_exit_status = 130;
		unlink(".heredoc_tmp");
	}
}

void	create_heredoc_open(const char *delimiter, t_command *cmd, t_env *env, bool *hrd_interrupted)
{
	
	pid_t	pid;
	int		status;

	signal(SIGINT, SIG_IGN);
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
		create_heredoc_effective(delimiter, cmd, env);
	waitpid(pid, &status, 0);
	signal(SIGINT, sigint_handler);
	heredoc_open_interrupted(status, hrd_interrupted);
}

void	command_not_found(t_command *cmd, t_env *env)
{
	if (cmd->argv)
		ft_putstr_fd(cmd->argv[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	g_exit_status = 127;
	free_env_cmdl_null(env, &cmd);
	exit(g_exit_status);
}

void	filter_args_fill(t_command *cmd, char ***argv_filtered, int count, int *j)
{
	int i;

	i = 0;
	while (i < count)
	{
		if (!cmd->arg_is_redir[i])
			(*argv_filtered)[(*j)++] = cmd->argv[i];
		i++;
	}
}

void	filter_args(t_command *cmd, char ***argv_filtered, t_env *env)
{
	int count;
	int argc_new;
	int i;
	int j;

	count = 0;
	while (cmd->argv[count])
		count++;
	argc_new = 0;
	i = 0;
	while (i < count)
	{
		if (!cmd->arg_is_redir[i])
			argc_new++;
		i++;
	}
	*argv_filtered = (char **)malloc(sizeof(char *) * (argc_new + 1));
	if (!*argv_filtered)
	{
		free_env_cmdl_null(env, &cmd);
		exit(1);
	}
	j = 0;
	filter_args_fill(cmd, argv_filtered, count, &j);
	(*argv_filtered)[j] = NULL;
}

void handle_child_cmd_path_exec_non_builtin(t_command *cmd, t_env *env, 
		char *cmd_path, char **argv_filtered)
{
	struct stat statbuf;
	
	if (stat(cmd_path, &statbuf) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(argv_filtered);
		free(cmd_path);
		exit(127);
	}
	if (S_ISDIR(statbuf.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		free(argv_filtered);
		free(cmd_path);
		exit(126);
	}
	if (access(cmd_path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		free(argv_filtered);
		free(cmd_path);
		exit(126);
	}
	filter_args(cmd, &argv_filtered, env);
	execve(cmd_path, argv_filtered, convert_env_list_to_array(env));

	perror(cmd_path);  // Cambia "execve" in cmd_path777777777777777777777777777777777777777777777777777777777777777
	
	free(argv_filtered);
	free(cmd_path);
	exit(1);
}

void	handle_child_cmd_path(t_command *cmd, t_env *env)
{
	char	*cmd_path;
	char	**argv_filtered;

	argv_filtered = NULL;
	if (cmd->argv)
		cmd_path = get_command_path(cmd->argv[0], env);
	else
		cmd_path = NULL;
	
	if (!cmd_path)
		command_not_found(cmd, env);  // Exit 127
	if (is_builtin(cmd))
	{
		free(cmd_path);
		exec_builtin(cmd, &env);
		free_env_cmdl_null(env, &cmd);
		exit(g_exit_status);
	}
	else
		handle_child_cmd_path_exec_non_builtin(cmd, env, cmd_path, argv_filtered);
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

void	handle_parent_process(int *prev_fd, int pipe_fd[])
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (pipe_fd[1] != -1)
	{
		close(pipe_fd[1]);
		*prev_fd = pipe_fd[0];
	}
	else
	{
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
	}
}

void	setup_pipe(t_command *cmd, int pipe_fd[], t_env *env)
{
	if (cmd->next && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		free_env_cmdl_null(env, &cmd);
		exit(EXIT_FAILURE);
	}
}

void wait_for_children(pid_t last_pid)
{
	int status;
	pid_t pid;

	while ((pid = wait(&status)) > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_exit_status = 128 + WTERMSIG(status);
			else
				g_exit_status = 1; // fallback
		}
	}
	if (pid == -1 && errno != ECHILD)
		perror("waitpid");
}

int	is_cmd_redir_in_2(t_command *cmd, int prev_fd, t_env *env, bool *hrd_interrupted)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			create_heredoc_open(r->filename, cmd, env, hrd_interrupted);//aggiunto env
			if (*hrd_interrupted)
			{
				*hrd_interrupted = false;
				if (prev_fd != -1)
					close(prev_fd);
				return (1);
			}
		}
		r = r->next;
	}
	return (0);
}

void	failed_pipe_with_next_cmd(t_command *cmd, int prev_fd, int *pipe_fd)
{
	if (cmd->next && pipe(pipe_fd) == -1)
	{
		perror("pipe");
		if (prev_fd != -1)
			close(prev_fd);
		return ;
	}
}

void	if_pid_minus_one(pid_t pid, int prev_fd, int *pipe_fd)
{
	if (pid == -1)
	{
		perror("fork");
		if (prev_fd != -1)
			close(prev_fd);
		if (pipe_fd[0] != -1)
			close(pipe_fd[0]);
		if (pipe_fd[1] != -1)
			close(pipe_fd[1]);
		return ;
	}
}

void exec_command_list(t_command *cmd_list, t_env *env, bool *g)
{
	t_command *cmd = cmd_list;
	t_p_fd p_fd;
	p_fd.prev_fd = -1;
	pid_t pid;
	pid_t last_pid = -1;

	while (cmd)
	{
		if (is_cmd_redir_in_2(cmd, p_fd.prev_fd, env, g))
		{
			return ;
		}
		p_fd.pipe_fd[0] = -1;
		p_fd.pipe_fd[1] = -1;
		if (cmd->next && pipe(p_fd.pipe_fd) == -1)
		{
			perror("pipe");
			if (p_fd.prev_fd != -1)
				close(p_fd.prev_fd);
			return ;
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			if (p_fd.prev_fd != -1) close(p_fd.prev_fd);
			if (p_fd.pipe_fd[0] != -1) close(p_fd.pipe_fd[0]);
			if (p_fd.pipe_fd[1] != -1) close(p_fd.pipe_fd[1]);
			return ;
		}
		last_pid = pid;
		if (pid == 0)
		{
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			handle_child_process(cmd, p_fd, env);
			free_env_cmdl_null(env, &cmd_list);
			exit(1);
		}
		else //genitore
		{
			if (p_fd.prev_fd != -1)
				close(p_fd.prev_fd);
			if (p_fd.pipe_fd[1] != -1)
			{
				close(p_fd.pipe_fd[1]);
				p_fd.prev_fd = p_fd.pipe_fd[0];
			}
			else if (p_fd.pipe_fd[0] != -1)
				close(p_fd.pipe_fd[0]);
			cmd = cmd->next;
		}
	}
	wait_for_children(last_pid);
}
