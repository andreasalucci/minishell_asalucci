#include "../libft/libft.h"
#include "../minishell.h"

static void	heredoc_sigint_handler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	exit(130);
}

void apply_redirections(t_command *cmd, t_env *env, t_global *global)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_IN)
			apply_redir_in1(r, env, cmd, global);
		else if (r->type == REDIR_OUT)
			apply_redir_out1(r, env, cmd, global);
		else if (r->type == REDIR_APPEND)
			apply_redir_out2(r);
		else if (r->type == REDIR_HEREDOC)
			apply_redir_heredoc();
		r = r->next;
	}
}

void	apply_redir_in1(t_redir *r, t_env *env, t_command *cmd, t_global *global)
{
	int	fd;

	fd = open(r->filename, O_RDONLY);
	if (fd < 0)
	{
		perror(r->filename);
		cleanup_resources(env, global);
		free_command_l(cmd);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	apply_redir_out1(t_redir *r, t_env *env, t_command *cmd, t_global *global)
{
	int	fd;

	fd = open(r->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(r->filename);
		cleanup_resources(env, global);
		free_command_l(cmd);
		exit(EXIT_FAILURE);
	}
	if (r->next && (r->next->type == REDIR_OUT || r->next->type == REDIR_APPEND))
	{
		//printf("DEBUG 222222222\n\n");
		close(fd);
	}
	else
	{
		//printf("DEBUG 3333333\n\n");
		dup2(fd, STDOUT_FILENO);
		close(fd);
		//close(STDOUT_FILENO);
	}
}

void	apply_redir_out2(t_redir *r)
{
	int	fd;

	fd = open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(r->filename);
		exit(EXIT_FAILURE);
	}
	if (r->next && (r->next->type == REDIR_OUT || r->next->type == REDIR_APPEND))
		close(fd);
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}

void apply_redir_heredoc(void)
{
	int fd;

	fd = open(".heredoc_tmp", O_RDONLY);
	if (fd < 0)
	{
		perror(".heredoc_tmp");
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	create_heredoc_effective(const char *delimiter)
{
	int		fd;
	char	*line;

	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open .heredoc_tmp");
		exit(1);
	}
	
	signal(SIGINT, heredoc_sigint_handler);
	
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			write(STDOUT_FILENO, "\n", 1);
			break;
		}
		
		// SALVA OGNI LINEA NELLA HISTORY!
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
	exit(0);
}

void	heredoc_open_interrupted(int status, t_global *g)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		g->heredoc_interrupted = 1;
		g_exit_status = 130;
		unlink(".heredoc_tmp");
	}
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		g->heredoc_interrupted = 1;
		g_exit_status = 130;
		unlink(".heredoc_tmp");
	}
}

void	create_heredoc_open(const char *delimiter, t_global *g)
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
		create_heredoc_effective(delimiter);
	waitpid(pid, &status, 0);
	signal(SIGINT, sigint_handler);
	heredoc_open_interrupted(status, g);
}

void	command_not_found(t_command *cmd)
{
	ft_putstr_fd(cmd->argv[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	g_exit_status = 127;
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

void	filter_args(t_command *cmd, char ***argv_filtered)
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
		exit(1);
	j = 0;
	filter_args_fill(cmd, argv_filtered, count, &j);
	(*argv_filtered)[j] = NULL;
}

void	handle_child_cmd_path(t_command *cmd, t_env *env, t_global *global)
{
	char	*cmd_path;
	char	**argv_filtered;

	cmd_path = get_command_path(cmd->argv[0], env);
	if (!cmd_path)
		command_not_found(cmd);
	if (is_builtin(cmd))
	{
		free(cmd_path);
		exec_builtin(cmd, &env);
		cleanup_resources(env, global);
		free_command_l(cmd);
		exit(g_exit_status);
	}
	else
	{
		filter_args(cmd, &argv_filtered);
		execve(cmd_path, argv_filtered, convert_env_list_to_array(env));
		perror("execve");
		free(argv_filtered);
		free(cmd_path);
		
		exit(126);
	}
}

void	handle_child_process(t_command *cmd, t_p_fd p_fd, t_env *env, t_global* global)
{
	//int prev_fd = -1; //creato adesso
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
	apply_redirections(cmd, env, global);
	handle_child_cmd_path(cmd, env, global);
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

void	setup_pipe(t_command *cmd, int pipe_fd[])
{
	if (cmd->next && pipe(pipe_fd) == -1)
	{
		perror("pipe");
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




int	is_cmd_redir_in_2(t_command *cmd, int prev_fd, t_global *g)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			create_heredoc_open(r->filename, g);
			if (g->heredoc_interrupted)
			{
				g->heredoc_interrupted = 0;
				//ft_printf("prev_fd:: %i\n", prev_fd);
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

/*void	sigdfl_handle_child_process(t_command *cmd, int prev_fd, int *pipe_fd,
		t_env *env)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	
	handle_child_process(cmd, prev_fd, pipe_fd, env);
}*/




void exec_command_list(t_command *cmd_list, t_env *env, t_global *g)
{
	t_command *cmd = cmd_list;
	t_p_fd p_fd;
	p_fd.prev_fd = -1;
	pid_t pid;
	pid_t last_pid = -1;
	

	while (cmd)
	{
		if (is_cmd_redir_in_2(cmd, p_fd.prev_fd, g))
			return ;

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
			handle_child_process(cmd, p_fd, env, g);
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