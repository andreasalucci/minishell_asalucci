#include "minishell.h"
#include "libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAX_INPUT
#define MAX_INPUT 1024
#endif

// Funzione di parsing semplice: divide una stringa in argomenti separati da spazio
// char **parse_input(char *input) {
//     char **args = malloc(sizeof(char *) * 64);
//     int i = 0;
//     char *token = strtok(input, " \t\r\n");
//     while (token != NULL) {
//         args[i++] = strdup(token);
//         token = strtok(NULL, " \t\r\n");
//     }
//     args[i] = NULL;
//     return args;
// }

// Inizializzazione ambiente minimo
t_env *init_env() {
    t_env *env = NULL;
    add_env(&env, "PWD", "/home/user", 1);
    add_env(&env, "HOME", "/home/user", 1);
    return env;
}

int main()//int argc, char *argv[], char **envp)
{
	// (void)argc;
	// (void)argv;
	// (void)envp;
	char *input;
	t_t *token;
	t_env *env = init_env();
	t_command *cmds;


    // printf("Shell Built-in Test - type 'exit' to quit\n");

	while(1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			ft_printf("exit\n");
			break ;
		}
		if (strncmp(input, "exit", 4) == 0) ////// Forse anche con Ctrl C freeare come fa in fondo V V V
            break;
		if (*input != '\0')
			add_history(input);
		token = tokens(input);
		if (token)
		{
    		parse(token);
			
			cmds = parse_commands(token);
			if (!cmds)
				return 0;
			printf("(cmds->argv[0]:: %s)\n", cmds->argv[0]);
			// if (!fgets(input, MAX_INPUT, stdin))
			// 	break;
			// if (strncmp(input, "exit", 4) == 0) ////// Forse anche con Ctrl C freeare come fa in fondo V V V
			// 	break;
			if (cmds->argv[0] == NULL) {
				free(cmds->argv);
				continue;
			}
			
			if (strcmp(cmds->argv[0], "cd") == 0) {
				builtin_cd(cmds->argv, &env);
			} else if (strcmp(cmds->argv[0], "export") == 0) {
				builtin_export(cmds->argv, &env);
			} else if (strcmp(cmds->argv[0], "env") == 0) {
				builtin_env(env);
			} else if (strcmp(cmds->argv[0], "unset") == 0) {
				builtin_unset(cmds->argv, &env);
			} else if (strcmp(cmds->argv[0], "pwd") == 0) {
				builtin_pwd();
			} else if (strcmp(cmds->argv[0], "echo") == 0) {
				int i = 0;
				while (cmds->argv[i])
				{
					i++;
					builtin_echo(i, cmds->argv);
				}
			} else {
				printf("Unknown command: %s\n", cmds->argv[0]);
			}
			for (int i = 0; cmds->argv[i]; i++)
				free(cmds->argv[i]);
			free(cmds->argv);

		}
		free(input);
	}
    free_env(env);
    return 0;
}
