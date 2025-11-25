#include "../minishell.h"

void	process_commands(t_command *cmds, t_env **env, bool *hdc_interrupted)
{
	if (!cmds)
		return ;
	if (cmds->argv && cmds->argv[0])
	{
		if (first_arg_is_one_dot(cmds->argv[0])
			|| first_arg_is_dotslash(cmds->argv[0])
			|| first_arg_is_all_dots(cmds->argv[0]))
		{
			free_command_l(cmds);
			return ;
		}
		if (ft_strcmp(cmds->argv[0], "exit") == 0)
		{
			free_env(*env);
			builtin_exit(cmds);
		}
	}
	if (!has_pipe_or_redir(cmds))
	{	
		exec_single_command(cmds, env);
		//if (cmds)
			free_env_cmdlnull_envp(NULL, &cmds, true, NULL);
	}
	else
		exec_command_list(cmds, *env, hdc_interrupted);
	// if (cmds)
	// 	free_env_cmdlnull_envp(NULL, &cmds, true, NULL);
}
