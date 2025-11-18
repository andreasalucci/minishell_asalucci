#include "../minishell.h"

bool	pipe_error(t_p_fd *p_fd, t_command *cmd)
{
	if (cmd->next && pipe((*p_fd).pipe_fd) == -1)
	{
		perror("pipe");
		if ((*p_fd).prev_fd != -1)
			close((*p_fd).prev_fd);
		return (true);
	}
	return (false);
}

bool	fork_error(pid_t pid, t_p_fd *p_fd)
{
	if (pid == -1)
	{
		perror("fork");
		if ((*p_fd).prev_fd != -1)
			close((*p_fd).prev_fd);
		if ((*p_fd).pipe_fd[0] != -1)
			close((*p_fd).pipe_fd[0]);
		if ((*p_fd).pipe_fd[1] != -1)
			close((*p_fd).pipe_fd[1]);
		return (true);
	}
	return (false);
}
