#include "../minishell.h"

void	filter_args_fill(t_command *cmd, char ***argv_filtered, int count,
		int *j)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (!cmd->arg_is_redir[i])
			(*argv_filtered)[(*j)++] = cmd->argv[i];
		i++;
	}
}

void	filter_args(t_command *cmd, char ***argv_filtered, t_env *env)
{
	int	count;
	int	argc_new;
	int	i;
	int	j;

	count = 0;
	while (cmd->argv[count])
		count++;
	argc_new = 0;
	i = 0;
	while (i < count)
	{
		if (!cmd->arg_is_redir[i])
			argc_new++;
		i++;
	}
	*argv_filtered = (char **)malloc(sizeof(char *) * (argc_new + 1));
	if (!*argv_filtered)
	{
		free_env_cmdlnull_envp(env, &cmd, true, NULL);
		exit(1);
	}
	j = 0;
	filter_args_fill(cmd, argv_filtered, count, &j);
	(*argv_filtered)[j] = NULL;
}
