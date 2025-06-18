#include <unistd.h>
#include "libft/libft.h"

typedef struct s_env
{
    char *key;
    char *value;
	int exportable;
    struct s_env *next;
} t_env;

void    builtin_export(t_env **env, char *arg);
void print_export(t_env *env);
int add_env(t_env **env, char *key, char *value, int exportable);
int update_env(t_env **env, char *key, char *value);
int mark_as_exportable(t_env **env, char *key);
void free_env(t_env *env);
t_env *create_env_node(char *key, char *value, int exportable);
t_env *copy_env_node(t_env *src);
void insert_sorted(t_env **sorted, t_env *new_node);
t_env *sort_env_copy(t_env *env);
int	env_exists(t_env *env, const char *key);
