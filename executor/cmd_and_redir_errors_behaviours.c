#include "../minishell.h"

void	cmd_error_no_such_file_or_dir(char *cmd_path, char **argv_filtered)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	if (argv_filtered)
		free(argv_filtered);
	free(cmd_path);
	exit(127);
}

void	cmd_error_is_a_dir(char *cmd_path, char **argv_filtered)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_path, 2);
	ft_putstr_fd(": Is a directory\n", 2);
	if (argv_filtered)
		free(argv_filtered);
	free(cmd_path);
	exit(126);
}

void	cmd_error_permission_denied(char *cmd_path, char **argv_filtered)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd_path, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	if (argv_filtered)
		free(argv_filtered);
	free(cmd_path);
	exit(126);
}

void	redir_error_no_such_file_or_dir(t_redir *r)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(r->filename, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	g_exit_status = 1;
}

void	redir_error_not_a_dir(t_redir *r)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(r->filename, 2);
	ft_putstr_fd(": Not a directory\n", 2);
	g_exit_status = 1;
}
