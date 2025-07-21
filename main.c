#include "minishell.h"
#include "libft/libft.h"

#ifndef MAX_INPUT
#define MAX_INPUT 1024
#endif

int g_exit_status = 0;

t_env *init_env()
{
    t_env *env = NULL;
    add_env(&env, "PWD", "/home/user", 1);
    add_env(&env, "HOME", "/home/user", 1);
	add_env(&env, "PATH", "/bin:/usr/bin", 1);
    return env;
}

char *ft_strjoin_3(const char *s1, const char *s2, const char *s3)
{
    size_t len1;
    size_t len2;
    size_t len3;
    char *result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc(len1 + len2 + len3 + 1);
    if (!result)
        return NULL;
    ft_strcpy(result, s1);
    ft_strlcat(result, s2, len1 + len2 + len3 + 1);
    ft_strlcat(result, s3, len1 + len2 + len3 + 1);
    return result;
}

char	**convert_env_list_to_array(t_env *env)
{
    int count = 0;
    t_env *tmp = env;
	int i = 0;
	char **envp = malloc((count + 1) * sizeof(char *));
	
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }
    tmp = env;
    while (tmp)
    {
        envp[i] = ft_strjoin_3(tmp->key, "=", tmp->value); // ad esempio "PATH=/usr/bin"
        i++;
        tmp = tmp->next;
    }
    envp[i] = NULL;
    return envp;
}

bool is_builtin(t_command *cmd)
{
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return false;
    return (strcmp(cmd->argv[0], "cd") == 0
         || ft_strcmp(cmd->argv[0], "export") == 0
         || ft_strcmp(cmd->argv[0], "unset") == 0
         || ft_strcmp(cmd->argv[0], "exit") == 0
         || ft_strcmp(cmd->argv[0], "echo") == 0
         || ft_strcmp(cmd->argv[0], "env") == 0
         || ft_strcmp(cmd->argv[0], "pwd") == 0);
}

bool	has_pipe_or_redir(t_command *cmd)
{
    return (cmd->next != NULL
		|| cmd->redir_in != 0
		|| cmd->redir_out != 0);
}

char *ft_strjoin_free(char *s1, char *s2)
{
    char *joined = ft_strjoin(s1, s2);
    free(s1);
    return joined;
}

char *get_command_path(char *cmd, t_env *env)
{
    char    *path_var;
    char    **paths;
    char    *candidate;
    int     i;
    // Se il comando contiene uno slash, è già un path esplicito
    if (ft_strchr(cmd, '/'))
        return ft_strdup(cmd);
    path_var = get_env_value(env, "PATH");
    if (!path_var)
        return NULL;
    paths = ft_split(path_var, ':');
    if (!paths)
        return NULL;
    i = 0;
    while (paths[i])
    {
        candidate = ft_strjoin(paths[i], "/");
        candidate = ft_strjoin_free(candidate, cmd); // funzione che libera il primo argomento
        if (access(candidate, X_OK) == 0)
            //free_split(paths);
            return candidate;
        //free(candidate);
        i++;
    }
    //free_split(paths);
    return NULL;
}

void free_env_array(char **envp)
{
    int i = 0;

    while (envp[i] != NULL)
    {
        free(envp[i]);
        i++;
    }
    free(envp);
}

void	exec_builtin(t_command *cmds, t_env **env)
{
	//char *status_str = ft_itoa(g_exit_status);

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
    else if (ft_strcmp(cmds->argv[0], "echo") == 0)// && !(cmds->argv[1] && ft_strcmp(cmds->argv[1], "$?") == 0))
        builtin_echo(cmds);
	// else if (ft_strcmp(cmds->argv[0], "echo") == 0 && cmds->argv[1] && ft_strcmp(cmds->argv[1], "$?") == 0)
	// {
	// 	printf("%s\n", status_str);
	// 	free(status_str);
	// 	g_exit_status = 0;
	// }
}

char *expand_exit_status(char *input) // devi fare la fuznione generale per le $
{
    char *pos = strstr(input, "$?"); /////////////////////////////////////INVALID FUNC
    if (!pos)
        return ft_strdup(input);  // nessuna espansione necessaria

    char *before = ft_substr(input, 0, pos - input);
    char *after = ft_strdup(pos + 2);
    char *status_str = ft_itoa(g_exit_status);

    char *temp = ft_strjoin(before, status_str);
    char *expanded = ft_strjoin(temp, after);

    free(before);
    free(after);
    free(temp);
    free(status_str);

    return expanded;
}

void	exec_single_non_builtin(t_command *cmds, t_env **env)
{
	char *cmd_path;
	char **envp = convert_env_list_to_array(*env);
	int status;

	cmd_path = get_command_path(cmds->argv[0], *env);
	if (cmd_path)
	{
		pid_t pid = fork();
		if (pid == 0) // siamo nel figlio
		{
			execve(cmd_path, cmds->argv, envp);
			perror("execve");
			free_env_array(envp);
			free(cmd_path);
			exit(EXIT_FAILURE);            // importante: usciamo dal figlio!
		}
		else if (pid > 0) // processo padre
		{
			waitpid(pid, NULL, 0); // aspettiamo il figlio
			if (WIFEXITED(status))
				g_exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				g_exit_status = 128 + WTERMSIG(status);
		}
		else
			perror("fork");
		free(cmd_path); // libero nel padre, solo il path
	}
	else
		printf("Command not found: %s\n", cmds->argv[0]);
}

int	main()
{
	char *input;
	t_t *token;
	t_env *env = init_env();
	t_command *cmds;

    // printf("Shell Built-in Test - type 'exit' to quit\n");

	while (1)
	{
		input = readline("minishell$ ");
		if (!input || strncmp(input, "exit", 4) == 0)
		{
			printf("exit\n");
			break;
		}
		if (*input)
			add_history(input);
		input = expand_exit_status(input); // trasformo gia qui ogni $?, cosa da cambiare 
											/*		Test  15: ❌ echo "exit_code ->$? user ->$USER home -> $HOME" 
											mini output = (exit_code ->0 user ->$USER home -> $HOME)
											bash output = (exit_code ->0 user ->asalucci home -> /home/asalucci)
											Test  16: ❌ echo 'exit_code ->$? user ->$USER home -> $HOME' 
											mini output = (exit_code ->0 user ->$USER home -> $HOME)
											bash output = (exit_code ->$? user ->$USER home -> $HOME)
											Test  17: ✅ echo "$" 
											Test  18: ✅ echo '$' 
											Test  19: ❌ echo $ 
											mini output = ()
											bash output = ($)
											Test  20: ✅ echo $? 
											Test  21: ✅ echo $?HELLO */

		token = tokens(input);
		if (token)
		{
			parse(token);
			cmds = parse_commands(token);
			if (!cmds)
				continue;
			if (!has_pipe_or_redir(cmds))
			{
				if(is_builtin(cmds))
					exec_builtin(cmds, &env);
				else
					exec_single_non_builtin(cmds, &env);
				continue;
			}
			else
			{
				//char **envp = convert_env_list_to_array(env);
				exec_command_list(cmds, env);  // gestisce pipe, fork, redir
				//free_env_array(envp);
			}
			free_command(cmds);  // cleanup eventuale
		}
		free(input);
	}
    free_env(env);
    return 0;
}
