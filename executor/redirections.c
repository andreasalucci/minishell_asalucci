#include "../minishell.h"
#include "../libft/libft.h"

void apply_redirections(t_command *cmd)
{
	if (cmd->infile && cmd->redir_in == 1)
		apply_redir_in1(cmd);
	if (cmd->infile && cmd->redir_in == 2)
		apply_redir_in2();
	if (cmd->outfile && cmd->redir_out == 1)
		apply_redir_out1(cmd);
	if (cmd->outfile && cmd->redir_out == 2)
		apply_redir_out2(cmd);
}

void apply_redir_in1(t_command *cmd)
{
	int fd;

	fd = open(cmd->infile, O_RDONLY);
	if (fd < 0)
	{
		perror(cmd->infile);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void apply_redir_in2(void)
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

void apply_redir_out1(t_command *cmd)
{
	int fd;

	fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(cmd->outfile);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void apply_redir_out2(t_command *cmd)
{
	int fd;

	fd = open(cmd->outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd < 0)
	{
		perror(cmd->outfile);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void create_heredoc_open(const char *delimiter)
{
	int		fd;
	char	*line;

	line = "\0";
	fd = open(".heredoc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("heredoc open");
		exit(EXIT_FAILURE);
	}
	create_heredoc_effective(delimiter, fd, line);
	close(fd);
}

void create_heredoc_effective(const char *delimiter, int fd, char *line)
{
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(fd);
			exit(EXIT_FAILURE); //attenzione a leak, non ci sono perche nessun malloc ma..
		}
		// Rimuovi newline finale da line
		char *newline = ft_strchr(line, '\n');
		if (newline)
			*newline = '\0';
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

void handle_child_process(t_command *cmd, int prev_fd, int pipe_fd[], t_env *env)//char **envp)
{
	int g_exit_status = 0;

    if (prev_fd != -1)
    {
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }
    if (cmd->next)
    {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
    }
    apply_redirections(cmd);
    char *cmd_path = get_command_path(cmd->argv[0], env);
	if (!cmd_path)
	{
		fprintf(stderr, "Command not found: %s\n", cmd->argv[0]);
		exit(EXIT_FAILURE);
	}
	if (is_builtin(cmd))
	{
		exec_builtin(cmd, &env); // se sei in un figlio, esci dopo
		exit(g_exit_status);
	}
	else
	{
		execve(cmd_path, cmd->argv, convert_env_list_to_array(env));
		perror("execve");
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
}

void handle_parent_process(int *prev_fd, int pipe_fd[])
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

void setup_pipe(t_command *cmd, int pipe_fd[])
{
    if (cmd->next && pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
}

void fork_process(pid_t *pid)
{
    *pid = fork();
    if (*pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}

void wait_for_children(void)
{
    while (wait(NULL) > 0);
}

void	exec_command_list(t_command *cmd_list, t_env *env)
{
    t_command	*cmd = cmd_list;
    int			pipe_fd[2];
    int			prev_fd = -1;
    pid_t		pid;

    while (cmd)
    {
        if (cmd->redir_in == 2)
            create_heredoc_open(cmd->infile);
        pipe_fd[0] = -1;
        pipe_fd[1] = -1;
        setup_pipe(cmd, pipe_fd);
        fork_process(&pid);
        if (pid == 0)
            handle_child_process(cmd, prev_fd, pipe_fd, env);
        else
            handle_parent_process(&prev_fd, pipe_fd);
        cmd = cmd->next;
    }
    wait_for_children();
}
