#include "../minishell.h"

void	exec_cmd_list_clean_hdc_1(t_command *head, t_command **cmd_list_ptr)
{
	cleanup_heredoc_files(head);
	free_command_l(head);
	*cmd_list_ptr = NULL;
	set_sigint_main();
}

void	exec_cmd_list_clean_hdc_2(t_command *head, t_command **cmd_list_ptr)
{
	cleanup_heredoc_files(head);
	free_command_l(head);
	*cmd_list_ptr = NULL;
}

void	exec_cmd_list_clean(t_command *head, t_command **cmd_list_ptr)
{
	cleanup_heredoc_files(head);
	free_command_l(head);
	*cmd_list_ptr = NULL;
}

bool	exec_command_list_2(t_command **cmd_list_ptr, t_command *head,
		t_env *env)
{
	t_p_fd		p_fd;
	pid_t		pid;
	pid_t		last_pid;
	t_command	*cmd;

	p_fd.prev_fd = -1;
	last_pid = -1;
	cmd = head;
	set_sigint_executing();
	while (cmd)
	{
		p_fd.pipe_fd[0] = -1;
		p_fd.pipe_fd[1] = -1;
		if (pipe_error(&p_fd, cmd))
			return (exec_cmd_list_clean(head, cmd_list_ptr), false);
		pid = fork();
		if (fork_error(pid, &p_fd))
			return (exec_cmd_list_clean(head, cmd_list_ptr), false);
		last_pid = pid;
		child_and_parent_process(pid, &cmd, &p_fd, env);
	}
	wait_for_children(last_pid);
	return (true);
}

void	exec_command_list(t_command **cmd_list_ptr, t_env *env,
							bool *hdc_interrupted)
{
	t_command	*head;

	if (!cmd_list_ptr || !*cmd_list_ptr)
		return ;
	head = *cmd_list_ptr;
	if (process_all_heredocs(head, env, hdc_interrupted))
		return (exec_cmd_list_clean_hdc_1(head, cmd_list_ptr));
	if (*hdc_interrupted)
		return (exec_cmd_list_clean_hdc_2(head, cmd_list_ptr));
	if (!exec_command_list_2(cmd_list_ptr, head, env))
		return ;
	set_sigint_main();
	*cmd_list_ptr = NULL;
}
