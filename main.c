#include "libft/libft.h"
#include "minishell.h"

int g_exit_status = 0;////////////////

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
	return (env);
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
			|| ft_strcmp(cmd->argv[0],
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

int	exec_and_wait(t_command *cmds, char *cmd_path, char **envp, t_env *env)
{
	int		status;
	pid_t	pid;
	struct	stat statbuf;

	pid = fork();
	if (pid == 0)
	{
		if (stat(cmd_path, &statbuf) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_path, 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			if (envp) free_env_array(envp);
			free(cmd_path);
			free_env(env);
			free_command_l(cmds);
			exit(127);
		}
		if (S_ISDIR(statbuf.st_mode))
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_path, 2);
			ft_putstr_fd(": Is a directory\n", 2);
			if (envp) free_env_array(envp);
			free(cmd_path);
			free_env(env);
			free_command_l(cmds);
			exit(126);
		}
		if (access(cmd_path, X_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_path, 2);
			ft_putstr_fd(": Permission denied\n", 2);
			if (envp) free_env_array(envp);
			free(cmd_path);
			free_env(env);
			free_command_l(cmds);
			exit(126);
		}
		execve(cmd_path, cmds->argv, envp);
		perror(cmd_path);
		if (envp)
			free_env_array(envp);
		free(cmd_path);
		free_env(env);
		free_command_l(cmds);
		exit(1);
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
	return 0;
}

void	exec_single_non_builtin(t_command *cmds, t_env **env)
{
	char	*cmd_path;
	char	**envp;

	envp = convert_env_list_to_array(*env);
	cmd_path = get_command_path(cmds->argv[0], *env);
	if (cmd_path)
	{
		if(exec_and_wait(cmds, cmd_path, envp, *env))
		{
			free_env(*env);
			return;
		}
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

int	handle_input_interruption(bool *hrd_interrupted, char *input)
{
	if (*hrd_interrupted)
	{
		*hrd_interrupted = false;
		free(input);
		input = NULL;
		return (1);
	}
	return (0);
}

int	handle_eof(char *input)
{
	if (!input)
	{
		free(input);
		input = NULL;
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

void	exec_single_command(t_command *cmds, t_env **env)
{
	if (is_builtin(cmds))
		exec_builtin(cmds, env);
	else
		exec_single_non_builtin(cmds, env);
}

void process_commands(t_command *cmds, t_env **env, bool *hrd_interrupted)
{
    if (!cmds)
        return;
    if (cmds->argv && cmds->argv[0])
    {
        if (first_arg_is_one_dot(cmds->argv[0]) || 
            first_arg_is_dot_slash(cmds->argv[0]) || 
            first_arg_is_all_dots(cmds->argv[0]))
        {
            free_command_l(cmds);
            return;
        }
        if (ft_strcmp(cmds->argv[0], "exit") == 0)
        {
            free_env(*env);
            builtin_exit(cmds);
        }
    }
    if (!has_pipe_or_redir(cmds))
        exec_single_command(cmds, env);
    else
        exec_command_list(cmds, *env, hrd_interrupted);
    free_command_l(cmds);
}

void	process_loop(char **input, t_env **env, bool *free_input, bool *hrd_interrupted)
{
	t_command	*cmds;

	process_input_history(*input);
	cmds = parse_input_to_commands(*input, free_input, *env);
	if(!cmds)
		free_env_cmdl_null(NULL, &cmds);
	process_commands(cmds, env, hrd_interrupted);
	if (*free_input)
	{
		*input = NULL;
		*free_input = false;
	}
}

int main_loop(t_env **env, bool *hrd_interrupted)
{
	char		*input;
	bool		free_input;
	char 		*prompt;

	input = NULL;
	free_input = 0;
	prompt = "\001\033[1;36m\002minishell\001\033[0m\002$ "; //prompt = "\001\033[1;36m\002minishell$ \001\033[0m\002";		Se deve essere verde
	while (1)
	{
		input = readline(prompt);
		if (input == NULL) //exit
			break ;
		if (input_is_open(input, env))
			return (2);
		if (handle_input_interruption(hrd_interrupted, input))
			continue ;
		if (handle_eof(input))
			break ;
		process_loop(&input, env, &free_input, hrd_interrupted);
	}
	rl_clear_history();
	return 0;
}

int	c_mode(char **argv, t_env **env, bool *hrd_interrupted)
{
	char *input;
	char *input_copy;
	t_command *cmds;

	input = argv[2];
	input_copy = strdup(input);
	if (!input_copy)
	{
		perror("strdup");
		free_env(*env);
		return (1);
	}
	if (input_is_open(input_copy, env))
		return (2);
	process_input_history(input_copy);
	bool free_input = false;
	cmds = parse_input_to_commands(input_copy, &free_input, *env);
	process_commands(cmds, env, hrd_interrupted);
	if (free_input)
		free(input_copy);
	free_env(*env);
	return (g_exit_status);
}

int main(int argc, char **argv, char **envp)
{
	t_env *env;
	bool hrd_interrupted;

	env = copy_env(envp);
	init_shlvl(&env);
	hrd_interrupted = false;
	setup_shell_signals();
	if (argc >= 3 && !ft_strncmp(argv[1], "-c", 3))
		return (c_mode(argv, &env, &hrd_interrupted));
	main_loop(&env, &hrd_interrupted);
	free_env(env);
	return 0;
}

