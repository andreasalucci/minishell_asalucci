#include "../minishell.h"

void	apply_redirections(t_command *cmd, t_env *env)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_IN)
			apply_redir_in(r, env, cmd);
		else if (r->type == REDIR_OUT)
			apply_redir_out1(r, env, cmd);
		else if (r->type == REDIR_APPEND)
			apply_redir_out2(r, cmd, env);
		r = r->next;
	}
}

void	apply_redir_in(t_redir *r, t_env *env, t_command *cmd)
{
	int	fd;

	fd = open(r->filename, O_RDONLY);
	if (fd < 0)
	{
		if (errno == ENOENT)
			redir_error_no_such_file_or_dir(r);
		else if (errno == ENOTDIR)
			redir_error_not_a_dir(r);
		else
		{
			perror(r->filename);
			g_exit_status = 1;
		}
		free_env_cmdlnull_envp(env, &cmd, true, NULL);
		exit(g_exit_status);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	if (r->filename && ft_strnstr(r->filename, "minish_hd_",
			ft_strlen(r->filename)))
		unlink(r->filename);
}

void	apply_redir_out1(t_redir *r, t_env *env, t_command *cmd)
{
	int	fd;

	fd = open(r->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		if (errno == ENOTDIR)
			redir_error_not_a_dir(r);
		else
		{
			perror(r->filename);
			g_exit_status = 1;
		}
		free_env_cmdlnull_envp(env, &cmd, true, NULL);
		exit(g_exit_status);
	}
	if (r->next && (r->next->type == REDIR_OUT
			|| r->next->type == REDIR_APPEND))
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
			redir_error_not_a_dir(r);
		else
		{
			perror(r->filename);
			g_exit_status = 1;
		}
		free_env_cmdlnull_envp(env, &cmd, true, NULL);
		exit(g_exit_status);
	}
	if (r->next && (r->next->type == REDIR_OUT
			|| r->next->type == REDIR_APPEND))
		close(fd);
	else
	{
		dup2(fd, STDOUT_FILENO);
		close(fd);
	}
}
