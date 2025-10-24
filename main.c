#include "libft/libft.h"
#include "minishell.h"

#ifndef MAX_INPUT
# define MAX_INPUT 1024
#endif

int	g_exit_status = 0;

t_command *init_command(void)
{
	t_command *cmd;
	
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->arg_is_redir = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_env	*init_env(void)
{
	t_env	*env;

	env = NULL;
	add_env(&env, "PWD", "/home/user", 1);
	add_env(&env, "HOME", "/home/user", 1);
	add_env(&env, "PATH", "/bin:/usr/bin", 1);
	//add_env(&env, "SHLVL", "0", 1);
	return (env);
}

t_env *copy_env(char **envp)
{
	t_env *env = NULL;
	int i = 0;
	char *eq;

	while (envp[i])
	{
		eq = ft_strchr(envp[i], '=');
		if (eq)
		{
			char *key = ft_substr(envp[i], 0, eq - envp[i]);
			char *value = ft_strdup(eq + 1);
			add_env(&env, key, value, 1);
			free(key);
			free(value);
		}
		i++;
	}
	return env;
}

char	*ft_strjoin_3(const char *s1, const char *s2, const char *s3)
{
	size_t	len1;
	size_t	len2;
	size_t	len3;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc(len1 + len2 + len3 + 1);
	if (!result)
		return (NULL);
	ft_strcpy(result, s1);
	ft_strlcat(result, s2, len1 + len2 + len3 + 1);
	ft_strlcat(result, s3, len1 + len2 + len3 + 1);
	return (result);
}

char	**convert_env_list_to_array(t_env *env)
{
	int		count;
	t_env	*tmp;
	int		i;
	char	**envp;

	count = 0;
	tmp = env;
	i = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = malloc((count + 1) * sizeof(char *));
	tmp = env;
	while (tmp)
	{
		envp[i] = ft_strjoin_3(tmp->key, "=", tmp->value);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

bool	is_builtin(t_command *cmd)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (false);
	return (ft_strcmp(cmd->argv[0], "cd") == 0 || ft_strcmp(cmd->argv[0],
			"export") == 0 || ft_strcmp(cmd->argv[0], "unset") == 0
		|| ft_strcmp(cmd->argv[0], "exit") == 0 || ft_strcmp(cmd->argv[0],
			"echo") == 0 || ft_strcmp(cmd->argv[0], "env") == 0
		|| ft_strcmp(cmd->argv[0], "pwd") == 0);
}

bool	has_pipe_or_redir(t_command *cmd)
{
	return (cmd->next != NULL || cmd->redirs != NULL);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

bool	dot_slash(char *cmd)
{
	if (ft_strcmp(cmd, ".") == 0)
		return (true);
	if (ft_strcmp(cmd, "./") == 0)
		return (true);
	return (false);
}

char	*get_command_path(char *cmd, t_env *env)
{
	char	*path_var;
	char	**paths;
	char	*candidate;
	int		i;

	if (dot_slash(cmd))
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_env_value(env, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		candidate = ft_strjoin(paths[i], "/");
		candidate = ft_strjoin_free(candidate, cmd);
		if (access(candidate, X_OK) == 0)
		{
			free_paths(paths);
			return (candidate);
		}
		free(candidate);	
		i++;
	}
	
	candidate = NULL;
	free_paths(paths);
	return (NULL);
}

void	free_env_array(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		free(envp[i]);
		i++;
	}
	free(envp);
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
	else if (ft_strcmp(cmds->argv[0], "exit") == 0)
		builtin_exit(cmds->argv);
}

bool	expand_exit_status(t_t *t)
{
	char	*before;
	char	*after;
	char	*status_str;
	char	*temp;
	char	*pos;

	pos = ft_strnstr(t->input + t->anchor_pos, "$?", ft_strlen(t->input
				+ t->anchor_pos));
	if (pos)
	{
		before = ft_substr(t->input, 0, pos - t->input);
		after = ft_strdup(pos + 2);
		status_str = ft_itoa(g_exit_status);
		temp = ft_strjoin(before, status_str);
		free(t->input);
		t->input = ft_strjoin(temp, after);
		free(before);
		free(after);
		free(temp);
		free(status_str);
		return (1);
	}
	else
		return (0);
}

void	exec_and_wait(t_command *cmds, char *cmd_path, char **envp)
{
	int		status;
	pid_t	pid;

	pid = fork();
	if (pid == 0)
	{
		//apply_redirections(cmds);
		execve(cmd_path, cmds->argv, envp);
		perror("execve");
		if (envp)
			free_env_array(envp);
		free(cmd_path);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			g_exit_status = 128 + WTERMSIG(status);
	}
	else
		perror("fork");
	free(cmd_path);
}

void	exec_single_non_builtin(t_command *cmds, t_env **env)
{
	char	*cmd_path;
	char	**envp;

	envp = convert_env_list_to_array(*env);
	cmd_path = get_command_path(cmds->argv[0], *env);
	if (cmd_path)
	{
		exec_and_wait(cmds, cmd_path, envp);
	}
	else if (ft_strcmp(cmds->argv[0], ".") == 0)
	{
		ft_putstr_fd(".: filename argument required\n", 2);
		g_exit_status = 2;
	}
	else if (ft_strcmp(cmds->argv[0], "./") == 0)
	{
		ft_putstr_fd("./: Is a directory\n", 2);
		g_exit_status = 126;
	}
	else
	{
		ft_putstr_fd(cmds->argv[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		g_exit_status = 127;
	}
	free_env_array(envp);
}

void	sigint_handler(int signum)
{
	(void)signum;
	g_exit_status = 130;
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "\n", 1);
		if (!rl_done)
		{
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
	}
}

void	setup_shell_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	handle_input_interruption(t_global *global, char *input)
{
	if (global->heredoc_interrupted)
	{
		global->heredoc_interrupted = 0;
		free(input);
		return (1);
	}
	return (0);
}

int	handle_eof(char *input)
{
	if (!input)
	{
		printf("exit\n");
		free(input);//////////////////////////////
		return (1);
	}
	return (0);
}

void	process_input_history(char *input)
{
	if (*input)
		add_history(input);
}

t_command *parse_input_to_commands(char *input, bool *free_input, t_env *env)
{
	t_t *token;
	t_command *cmd;

	token = tokens(input, free_input, env);  // PASS env
	if (!token)
		return (NULL);
	cmd = parse(token);
	//cmd = parse_commands(token);
   	free_token_list(token);  // ← Aquí liberas los tokens
	return (cmd);
}

void	execute_single_command(t_command *cmds, t_env **env)
{
	if (is_builtin(cmds))
		exec_builtin(cmds, env);
	else
		exec_single_non_builtin(cmds, env);
}

void process_commands(t_command *cmds, t_env **env, t_global *global)
{
	if (!cmds)
		return;
	if (!has_pipe_or_redir(cmds))
		execute_single_command(cmds, env);  // Passa env come doppio puntatore
	else
		exec_command_list(cmds, *env, global);  // Dereferenzia env	
	free_command_l(cmds);
}

void	cleanup_resources(t_env *env, t_global *global)
{
	free_env(env);
	free(global);
}

bool	only_spaces_after_pipe(char *pp)
{
	size_t i;

	i = 1;
	while (pp[i])
	{
		if (!(pp[i] == ' ' || pp[i] == '\t'))
			return (0);
		i++;
	}
	return (1);
}

bool quotes_closed(char *input)
{
	bool in_single;
	bool in_double;
	char *p;

	in_single = false;
	in_double = false;
	p = input;
	while (*p)
	{
		if (*p == '\'' && !in_double)
			in_single = !in_single;
		else if (*p == '"' && !in_single)
			in_double = !in_double;
		else if (*p == '\\')
			if (*(p + 1))
				p++;
		p++;
	}
	return (!in_single && !in_double);
}

int	input_is_open(char *input)
{
	char *pp;

	pp = ft_strchr(input, '|');
	if (!quotes_closed(input))
		return (1);
	if (pp && only_spaces_after_pipe(pp))
		return (2);
	return (0);
}

int main_loop(t_env **env, t_global *global)
{
	char        *input = NULL;
	t_command   *cmds;
	int			open_type;
	bool	free_input;

	free_input = 0;
	while (1)
	{
		input = readline("minishell$ ");
		if (input == NULL)
		{
			printf("exit\n");
			break;
		}
		open_type = input_is_open(input);


		if (open_type == 1)
		{
			ft_putstr_fd("minishell: Syntax error: unclosed quotes\n", 2);
			g_exit_status = 2;
			free(input);
			input = NULL;
			continue;
		}
		if (handle_input_interruption(global, input))
		{
			input = NULL;
			continue;
		}
		if (handle_eof(input))
		{
			input = NULL;
			break;
		}
		process_input_history(input);
		cmds = parse_input_to_commands(input, &free_input, *env);
		process_commands(cmds, env, global);
		if (free_input)
		{
			input = NULL;
			free_input = false;
		}
	}
	rl_clear_history();

	return 0;
}



int main(int argc, char **argv, char **envp)
{
	t_env *env = copy_env(envp);
	t_global *global = malloc(sizeof(t_global));

	if (!global)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	init_shlvl(&env);
	global->heredoc_interrupted = 0;
	setup_shell_signals();

	// Modo no interactivo con -c
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
	{
		char *input = argv[2];
		char *input_copy = strdup(input); // Duplica para evitar modificar argv[2]
		if (!input_copy)
		{
			perror("strdup");
			cleanup_resources(env, global);
			return 1;
		}

		int open_type = input_is_open(input_copy);
		if (open_type == 1)
		{
			ft_putstr_fd("minishell: Syntax error: unclosed quotes\n", 2);
			free(input_copy);
			cleanup_resources(env, global);
			return 2;
		}

		process_input_history(input_copy);
		bool free_input = false;
		t_command *cmds = parse_input_to_commands(input_copy, &free_input, env);
		process_commands(cmds, &env, global);

		if (free_input)
			free(input_copy);
		cleanup_resources(env, global);
		return g_exit_status;

	}
	
	// Modo interactivo normal
	main_loop(&env, global);
	cleanup_resources(env, global);
	
	return 0;
}

