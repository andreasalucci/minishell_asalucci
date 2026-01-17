#include "../minishell.h"

bool	command_particular_cases(t_command *cmds, t_env **env)
{
	if (!cmds)
		return (true);
	if (cmds->argv && cmds->argv[0] && !(cmds->argv[1]))
	{
		if (ft_strcmp(cmds->argv[0], "\0") == 0)
		{
			free_command_l(cmds);
			g_exit_status = 127;
			return (true);
		}
	}
	if (cmds->argv && cmds->argv[0])
	{
		if (first_arg_is_one_dot(cmds->argv[0])
			|| first_arg_is_dotslash(cmds->argv[0])
			|| first_arg_is_all_dots(cmds->argv[0]))
			return (free_command_l(cmds), true);
		if (ft_strcmp(cmds->argv[0], "exit") == 0)
		{
			free_env(*env);
			builtin_exit(cmds);
		}
	}
	return (false);
}

void	process_commands(t_command *cmds, t_env **env, bool *hdc_interrupted)
{
	t_command *c;

	if (command_particular_cases(cmds, env))
		return ;
	c = cmds;
	while (c)
	{
		if (c->contrasting_redirs)
		{
			cmds->contrasting_redirs = false;
			return (free_command_l(cmds));// (free_env(*env), free_arrarr(envp));
		}
		c = c->next;
	}
	if (!has_pipe_or_redir(cmds))
	{
		exec_single_command(cmds, env);
		free_env_cmdlnull_envp(NULL, &cmds, true, NULL);
	}
	else
		exec_command_list(&cmds, *env, hdc_interrupted);
}
