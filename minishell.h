#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft/libft.h"

typedef struct s_env
{
    char *key;
    char *value;
	int exportable;
    struct s_env *next;
} t_env;

int	builtin_cd(char **args, t_env **env);
void	builtin_env(t_env *env);
t_env	*copy_env_node(t_env *src);
t_env	*copy_env_list_sorted(t_env *env);
void	print_export(t_env *sorted);
int	handle_no_args(t_env *env);
int	handle_invalid_identifier(char *arg);
void	handle_key_value_pair(char *arg, t_env **env);
int	process_export_arg(char *arg, t_env **env);
int	builtin_export(char **args, t_env **env);
int	is_valid_identifier(const char *str);
int	unset_env(t_env **env, char *key);
int	builtin_unset(char **args, t_env **env);
void	free_env(t_env *env);
char	*get_env_value(t_env *env, const char *key);
int	mark_as_exportable(t_env **env, char *key);
t_env	*create_env_node(char *key, char *value, int exportable);
int	add_env(t_env **env, char *key, char *value, int exportable);
int	update_env(t_env **env, const char *key, const char *value);
void	update_env_var(t_env **env, const char *key, const char *value);
int	env_exists(t_env *env, const char *key);
void	insert_sorted(t_env **sorted, t_env *new_node);
int builtin_pwd(void);
int is_option_n(const char *str);
int builtin_echo(int argc, char **argv);