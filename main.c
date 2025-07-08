#include "minishell.h"
#include "libft/libft.h"

#ifndef MAX_INPUT
#define MAX_INPUT 1024
#endif

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
    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    size_t len3 = strlen(s3);
    char *result = malloc(len1 + len2 + len3 + 1);
    if (!result)
        return NULL;
    strcpy(result, s1);
    strcat(result, s2);
    strcat(result, s3);
    return result;
}

char	**convert_env_list_to_array(t_env *env)
{
    int count = 0;
    t_env *tmp = env;
    while (tmp)
    {
        count++;
        tmp = tmp->next;
    }

    char **envp = malloc((count + 1) * sizeof(char *));
    tmp = env;
    int i = 0;
    while (tmp)
    {
        envp[i] = ft_strjoin_3(tmp->key, "=", tmp->value); // ad esempio "PATH=/usr/bin"
        i++;
        tmp = tmp->next;
    }
    envp[i] = NULL;
    return envp;
}

// bool is_builtin(t_command *cmd)
// {
//     if (!cmd || !cmd->argv || !cmd->argv[0])
//         return false;
//     return (strcmp(cmd->argv[0], "cd") == 0
//          || strcmp(cmd->argv[0], "export") == 0
//          || strcmp(cmd->argv[0], "unset") == 0
//          || strcmp(cmd->argv[0], "exit") == 0
//          || strcmp(cmd->argv[0], "echo") == 0
//          || strcmp(cmd->argv[0], "env") == 0
//          || strcmp(cmd->argv[0], "pwd") == 0);
// }

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

    // Scorri l'array di stringhe e libera ogni stringa
    while (envp[i] != NULL)
    {
        free(envp[i]);
        i++;
    }

    // Alla fine, libera l'array stesso
    free(envp);
}

void	exec_single_simple_command(t_command *cmds, t_env **env)
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
    {
        int i = 0;
        while (cmds->argv[i])
            i++;
        builtin_echo(i, cmds->argv);
    }
    else
    {
        char *cmd_path = get_command_path(cmds->argv[0], *env);
        if (cmd_path)
        {
            char **envp = convert_env_list_to_array(*env);
            execve(cmd_path, cmds->argv, envp);
            perror("execve");
            free_env_array(envp);
            free(cmd_path);
            exit(EXIT_FAILURE); // execve ha fallito
        }
        else
            printf("Command not found: %s\n", cmds->argv[0]);
    }
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
		token = tokens(input);
		if (token)
		{
			parse(token);
			cmds = parse_commands(token);
			if (!cmds)
				continue;
			if (!has_pipe_or_redir(cmds))
				exec_single_simple_command(cmds, &env);
			else
			{
				char **envp = convert_env_list_to_array(env);
				exec_command_list(cmds, envp);  // gestisce pipe, fork, redir
				free_env_array(envp);
			}
			free_command(cmds);  // cleanup eventuale
		}
		free(input);
	}
    free_env(env);
    return 0;
}
