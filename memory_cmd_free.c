#include "minishell.h"

void	free_command_redirs(t_command **cmd)
{
	t_redir	*r;
	t_redir	*tmp;

	if (!cmd)
		return ;
	if (!*cmd)
		return ;
	// if (!(*cmd)->redirs)
	// 	return ;
	r = (*cmd)->redirs;
	while (r)
	{
		tmp = r->next;
		free(r->filename);
		free(r);
		r = tmp;
	}
	if ((*cmd)->infile)
	{
		free((*cmd)->infile);
		(*cmd)->infile = NULL;
	}
	if ((*cmd)->outfile)
	{
		free((*cmd)->outfile);
		(*cmd)->outfile = NULL;
	}
}

void	free_command(t_command **cmd)
{
	int	i;

	if (!cmd)
		return ;
	if (!(*cmd))
		return ;
	if ((*cmd)->arg_is_redir)
	{
		free((*cmd)->arg_is_redir);
		(*cmd)->arg_is_redir = NULL;
	}
	if ((*cmd)->argv)
	{
		i = 0;
		while ((*cmd)->argv[i])
		{
			free((*cmd)->argv[i++]);
			(*cmd)->argv[i] = NULL;
		}
		free((*cmd)->argv);
		(*cmd)->argv = NULL;
	}
	free_command_redirs(cmd);
	//printf("free_command 	before free:   %p   %p   %p\n", &cmd, cmd, *cmd);
	free(*cmd);
	(*cmd) = NULL;
	//printf("free_command	after NULL:   %p   %p   %p\n\n\n\n", &cmd, cmd, *cmd);
}



void	free_command_l(t_command **cmd_list)
{
    t_command *current;
    t_command *next;

	if (cmd_list)
	{
		if (*cmd_list)
		{
			current = *cmd_list;
			while (current)
			{
				next = current->next;
				free_command(&current);
				current = next;
			}
			*cmd_list = NULL;
		}
	}
}

// void	free_command_l(t_command **cmd_list)
// {
// 	t_command	*tmp;

// 	if (cmd_list)
// 	{
// 		while (*cmd_list)
// 		{
// 			tmp = (*cmd_list)->next;
// 			free_command(cmd_list);
// 			*cmd_list = tmp;
// 		}
// 	}
// }


void	free_env_cmdlnull_envp(t_env *env, t_command **cmd_list,
		bool nullify_cmd, char **envp)
{
	if (env)
		free_env(env);
	if (cmd_list)
	{
		if (*cmd_list)
		{
			free_command_l(cmd_list);
			if (nullify_cmd)
				*cmd_list = NULL;
		}
		if (nullify_cmd)	// AGGIUNTO QUESTO
			cmd_list = NULL;//     ma con o senza è uguale
	}
	if (envp)
		free_arrays_array(envp);
}
