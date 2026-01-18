#include "../minishell.h"

bool	is_builtin(t_command *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (false);
	return (ft_strcmp(cmd->argv[0], "cd") == 0 || ft_strcmp(cmd->argv[0],
			"export") == 0 || ft_strcmp(cmd->argv[0], "unset") == 0
		|| ft_strcmp(cmd->argv[0], "echo") == 0 || ft_strcmp(cmd->argv[0],
			"env") == 0 || ft_strcmp(cmd->argv[0], "pwd") == 0);
}

void	exec_builtin(t_command *cmds, t_env **env)
{
	if (ft_strcmp(cmds->argv[0], "cd") == 0)
		builtin_cd(cmds->argv, env);
	else if (ft_strcmp(cmds->argv[0], "export") == 0)
		builtin_export(cmds->argv, env);
	else if (ft_strcmp(cmds->argv[0], "env") == 0)
		builtin_env(*env);
	else if (ft_strcmp(cmds->argv[0], "unset") == 0)
		builtin_unset(cmds->argv, env);
	else if (ft_strcmp(cmds->argv[0], "pwd") == 0)
		builtin_pwd();
	else if (ft_strcmp(cmds->argv[0], "echo") == 0)
		builtin_echo(cmds);
}
