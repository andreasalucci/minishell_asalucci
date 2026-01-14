#include "minishell.h"

volatile sig_atomic_t	g_exit_status = 0;

void	process_loop(char **input, t_env **env, bool *free_input,
		bool *hdc_interrupted)
{
	t_command	*cmds;

	process_input_history(*input);
	cmds = parse_input_to_commands(*input, free_input, *env);
	if (!cmds)
		free_env_cmdlnull_envp(NULL, &cmds, true, NULL);
	process_commands(cmds, env, hdc_interrupted);
	if (*free_input)
	{
		*input = NULL;
		*free_input = false;
	}
}

void	main_loop_initializaton(char **input, bool *free_input,
						char **prompt, bool *hdc_interrupted)
{
	*input = NULL;
	*free_input = 0;
	*prompt = "\001\033[1;36m\002minishell\001\033[0m\002$ ";
	*hdc_interrupted = false;
}

int	main_loop(t_env **env, bool *hdc_interrupted)
{
	char	*input;
	bool	free_input;
	char	*prompt;

	main_loop_initializaton(&input, &free_input, &prompt, hdc_interrupted);
	while (1)
	{
		input = readline(prompt);
		if (input == NULL)
			break ;
		if (input_is_open(input))
		{
			process_input_history(input);
			free(input);
			continue ;
		}
		if (handle_input_interruption(hdc_interrupted, input))
			continue ;
		if (handle_eof(input))
			break ;
		process_loop(&input, env, &free_input, hdc_interrupted);
		*hdc_interrupted = false;
	}
	rl_clear_history();
	return (0);
}

int	c_mode(char **argv, t_env **env, bool *hdc_interrupted)
{
	char		*input;
	char		*input_copy;
	t_command	*cmds;
	bool		free_input;

	input = argv[2];
	input_copy = strdup(input);
	if (!input_copy)
	{
		perror("strdup");
		free_env(*env);
		return (1);
	}
	process_input_history(input_copy);
	input_is_open(input_copy);
	free_input = false;
	cmds = parse_input_to_commands(input_copy, &free_input, *env);
	process_commands(cmds, env, hdc_interrupted);
	if (free_input)
		free(input_copy);
	free_env(*env);
	return (g_exit_status);
}

int	main(int argc, char **argv, char **envp)
{
	t_env	*env;
	bool	hdc_interrupted;

	env = copy_env(envp);
	init_shlvl(&env);
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
		return (c_mode(argv, &env, &hdc_interrupted));
	set_sigint_main();
	signal(SIGQUIT, SIG_IGN);
	hdc_interrupted = false;
	main_loop(&env, &hdc_interrupted);
	free_env(env);
	return (g_exit_status);
}
