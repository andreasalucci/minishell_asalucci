#include "../minishell.h"

bool	heredoc_read_continues(char **line, t_command *cmd, t_env *env,
		const char *delimiter)
{
	*line = readline("> ");
	if (g_exit_status == 130)
	{
		free_env_cmdlnull_envp(env, &cmd, true, NULL);
		exit(130);
	}
	if (!(*line))
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_clear_history();
		return (false);
	}
	if (**line)
		add_history(*line);
	if (ft_strcmp(*line, delimiter) == 0)
	{
		free(*line);
		return (false);
	}
	return (true);
}

void	create_heredoc_effective(const char *delimiter, t_command *cmd,
		t_env *env)
{
	int		fd;
	char	*line;

	fd = open(".heredoc_tmp", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("open .heredoc_tmp");
		free_env_cmdlnull_envp(env, &cmd, true, NULL);
		exit(1);
	}
	signal(SIGINT, heredoc_sigint_handler);
	line = NULL;
	while (heredoc_read_continues(&line, cmd, env, delimiter))
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		fsync(fd);
		free(line);
	}
	close(fd);
	free_env_cmdlnull_envp(env, &cmd, true, NULL);
	exit(0);
}

void	heredoc_open_interrupted(int status, bool *hdc_interrupted)
{
	if (WIFEXITED(status) && WEXITSTATUS(status) == 130)
	{
		*hdc_interrupted = true;
		g_exit_status = 130;
		unlink(".heredoc_tmp");
	}
	else if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		*hdc_interrupted = true;
		g_exit_status = 130;
		unlink(".heredoc_tmp");
	}
}

void	create_heredoc_open(const char *delimiter, t_command *cmd, t_env *env,
		bool *hdc_interrupted)
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
	heredoc_open_interrupted(status, hdc_interrupted);
}
