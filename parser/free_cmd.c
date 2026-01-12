#include "../minishell.h"

void	free_command_redirs(t_command **cmd)
{
	t_redir	*r;
	t_redir	*tmp;

	r = (*cmd)->redirs;
	while (r)
	{
		tmp = r->next;
		free(r->filename);
		free(r);
		r = tmp;
	}
	free((*cmd)->infile);
	free((*cmd)->outfile);
}

void	free_command(t_command *cmd)
{
	int	i;

	i = 0;
	if (!cmd)
		return ;
	if (cmd->arg_is_redir)
	{
		free(cmd->arg_is_redir);
		cmd->arg_is_redir = NULL;
	}
	if (cmd->argv)
	{
		while (cmd->argv[i])
			free(cmd->argv[i++]);
		free(cmd->argv);
	}
	free_command_redirs(&cmd);
	free(cmd);
}

void	free_command_l(t_command *cmd_list)
{
	t_command	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		free_command(cmd_list);
		cmd_list = tmp;
	}
}

void	free_env_cmdlnull_envp(t_env *env, t_command **cmd_list,
		bool nullify_cmd, char **envp)
{
	if (env)
		free_env(env);
	if (cmd_list)
	{
		if (*cmd_list)
		{
			free_command_l(*cmd_list);
			if (nullify_cmd)
				*cmd_list = NULL;
		}
	}
	if (envp)
		free_arrays_array(envp);
}
