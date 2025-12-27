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

void	child_and_parent_process(pid_t pid, t_command **cmd, t_p_fd *p_fd,
		t_env *env)
{
	t_command	*tmp;

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
		tmp = (*cmd)->next;
		free_command(*cmd);
		*cmd = tmp;
	}
}




int	process_all_heredocs(t_command *cmd_list, t_env *env,
		bool *hdc_interrupted)
{
	t_command	*cmd;
	t_redir		*r;
	
	set_sigint_heredoc();

	cmd = cmd_list;
	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == REDIR_HEREDOC)
			{
				cmd_list->in_hdc = true;
				if (process_heredoc(r->filename, cmd, env, hdc_interrupted))
				{
					if (*hdc_interrupted)
					{
						cleanup_heredoc_files(cmd_list);
						g_exit_status = 130;
						return (130);
					}
					cleanup_heredoc_files(cmd_list);
					return (-1);
				}
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

void	cleanup_heredoc_files(t_command *cmd_list)
{
	t_command	*cmd;
	t_redir		*r;
	
	cmd = cmd_list;
	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->filename && ft_strnstr(r->filename, "minish_hd_", 
				ft_strlen(r->filename)) != NULL)
				unlink(r->filename);
			r = r->next;
		}
		cmd = cmd->next;
	}
}

void exec_command_list(t_command **cmd_list_ptr, t_env *env, bool *hdc_interrupted)
{
    t_p_fd      p_fd;
    pid_t       pid;
    pid_t       last_pid;
    t_command   *cmd;
    t_command   *head;

    if (!cmd_list_ptr || !*cmd_list_ptr)
        return;

    head = *cmd_list_ptr;

    /* PRIMA: Processa tutti gli heredoc SENZA FORK */
    if (process_all_heredocs(head, env, hdc_interrupted))
    {
        cleanup_heredoc_files(head);
        free_command_l(head);
        *cmd_list_ptr = NULL;
		set_sigint_main();///////////
        return;
    }
	if (*hdc_interrupted)
	{
        cleanup_heredoc_files(head);
        free_command_l(head);
        *cmd_list_ptr = NULL;
		return ;
	}
			// set_sigint_main();//COMMENTO CTRLCMOD

    /* POI: Esegui comandi normalmente */
    p_fd.prev_fd = -1;
    last_pid = -1;
    cmd = head;

	set_sigint_executing();
    while (cmd)
    {
        p_fd.pipe_fd[0] = -1;
        p_fd.pipe_fd[1] = -1;

        if (pipe_error(&p_fd, cmd))
        {
            cleanup_heredoc_files(head);
            free_command_l(head);
            *cmd_list_ptr = NULL;
            return;
        }

        pid = fork();
        if (fork_error(pid, &p_fd))
        {
            cleanup_heredoc_files(head);
            free_command_l(head);
            *cmd_list_ptr = NULL;
            return;
        }

        last_pid = pid;
        child_and_parent_process(pid, &cmd, &p_fd, env);
    }

    wait_for_children(last_pid);

									set_sigint_main();//CTRLCMOD

    //cleanup_heredoc_files(head);
    //free_command_l(head);
    *cmd_list_ptr = NULL;
	//head->in_hdc = false;
}



