
#include "minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT 1024

// Funzione di parsing semplice: divide una stringa in argomenti separati da spazio
char **parse_input(char *input) {
    char **args = malloc(sizeof(char *) * 64);
    int i = 0;
    char *token = strtok(input, " \t\r\n");
    while (token != NULL) {
        args[i++] = strdup(token);
        token = strtok(NULL, " \t\r\n");
    }
    args[i] = NULL;
    return args;
}

// Inizializzazione ambiente minimo
t_env *init_env() {
    t_env *env = NULL;
    add_env(&env, "PWD", "/home/user", 1);
    add_env(&env, "HOME", "/home/user", 1);
    return env;
}

int main() {
    char input[MAX_INPUT];
    char **args;
    t_env *env = init_env();

    printf("Shell Built-in Test - type 'exit' to quit\n");
    while (1) {
        printf(">> ");
        if (!fgets(input, MAX_INPUT, stdin))
            break;

        if (strncmp(input, "exit", 4) == 0) ////// Forse anche con Ctrl C freeare come fa in fondo V V V
            break;

        args = parse_input(input);
        if (args[0] == NULL) {
            free(args);
            continue;
        }

        if (strcmp(args[0], "cd") == 0) {
            builtin_cd(args, &env);
        } else if (strcmp(args[0], "export") == 0) {
            builtin_export(args, &env);
        } else if (strcmp(args[0], "env") == 0) {
            builtin_env(env);
        } else if (strcmp(args[0], "unset") == 0) {
            builtin_unset(args, &env);
		} else if (strcmp(args[0], "pwd") == 0) {
			builtin_pwd();
		} else if (strcmp(args[0], "echo") == 0) {
			int i = 0;
			while (args[i])
				i++;
			builtin_echo(i, args);
        } else {
            printf("Unknown command: %s\n", args[0]);
        }

        for (int i = 0; args[i]; i++) free(args[i]);
        free(args);
    }

    free_env(env);
    return 0;
}
