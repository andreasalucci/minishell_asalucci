#include "../minishell.h"

void	wait_for_children(pid_t last_pid)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_exit_status = 128 + WTERMSIG(status);
			else
				g_exit_status = 1;
		}
		pid = wait(&status);
	}
	if (pid == -1 && errno != ECHILD)
		perror("waitpid");
}

int	is_cmd_redir_in_2(t_command *cmd, int prev_fd, t_env *env,
		bool *hdc_interrupted)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			cmd->in_hdc = true;
			create_heredoc_open(r->filename, cmd, env, hdc_interrupted);
			if (*hdc_interrupted)
			{
				*hdc_interrupted = false;
				if (prev_fd != -1)
					close(prev_fd);
				return (1);
			}
		}
		r = r->next;
	}
	return (0);
}

void	child_and_parent_process(pid_t pid, t_command **cmd, t_p_fd *p_fd,
		t_env *env)
{
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
		t_command *tmp;
		tmp = (*cmd)->next;
		//printf("child and parent process	before:   %p   %p   %p\n", &cmd, cmd, *cmd);
		free_command(*cmd);
		//printf("child and parent process	after:   %p   %p   %p\n\n\n\n", &cmd, cmd, *cmd);
		*cmd = tmp;
		//cmd = &tmp;	non cambia niente	perche infatti free_command non distrugge t_command **cmd
		//NO FREE:	*cmd = (*cmd)->next;
	}
}

void	exec_command_list(t_command *cmd_list, t_env *env,
		bool *hdc_interrupted)
{
	t_p_fd		p_fd;
	pid_t		pid;
	pid_t		last_pid;
	t_command	*cmd;

	p_fd.prev_fd = -1;
	last_pid = -1;
	cmd = cmd_list;
	while (cmd)
	{
		if (is_cmd_redir_in_2(cmd, p_fd.prev_fd, env, hdc_interrupted))
			return ;
		p_fd.pipe_fd[0] = -1;
		p_fd.pipe_fd[1] = -1;
		if (pipe_error(&p_fd, cmd))
			return ;
		pid = fork();
		if (fork_error(pid, &p_fd))
			return ;
		last_pid = pid;
		child_and_parent_process(pid, &cmd, &p_fd, env);
	}
	wait_for_children(last_pid);
}
