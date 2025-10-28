#ifndef MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libft/libft.h"

#include <fcntl.h>
#include <sys/wait.h>
//#include <string.h>

#include <signal.h>
#include <errno.h>  // Per errno e ECHILD

#include <sys/stat.h>

extern int g_exit_status;

typedef struct s_p_fd
{
    int prev_fd;
	int pipe_fd[2];
}   t_p_fd;

typedef enum token_type
{
	METACHAR,
	TOKEN_DEFAULT,
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_HEREDOC,
	TOKEN_END,
	TOKEN_DOUBLE_REDIR_IN,
	TOKEN_DOUBLE_REDIR_OUT,
	TOKEN_VAR,
} t_token_type;

typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
    struct s_token  *prev;
	bool            single_quote;
    bool            double_quote;
    char            *input;
    char            *start;
    size_t          pos;
    size_t          anchor_pos;
    bool            error;
    size_t          quote;
    bool            continue_var;
    bool            to_remove; 
	char			*tmp_token;
	bool			free_input;
} t_t;

typedef enum e_redir_type
{
    REDIR_IN,      // <
    REDIR_OUT,     // >
    REDIR_APPEND,  // >>
    REDIR_HEREDOC  // <<
}   t_redir_type;

typedef struct s_redir
{
    t_redir_type type;
    char         *filename;
    struct s_redir *next;
}   t_redir;

typedef struct s_command 
{
    char				**argv;
	bool				*arg_is_redir;
    char				*infile;
    char				*outfile;
	t_redir				*redirs;
    // int					redir_in;
    // int					redir_out;
    struct s_command	*next;
} t_command;

typedef struct global 
{
	int		heredoc_interrupted;
} t_global;

typedef struct s_key_value {
    char    *key;
    char    *value;
    char    *new_value_part;
    char    *old_value;
} t_key_value;

typedef struct s_env
{
    char *key;
    char *value;
	int exportable;
    struct s_env *next;
} t_env;

t_t			*tokens(char *input, bool *free_input, t_env *env);
void		quotes(t_t *t);
void		metacharacters(t_t *t, t_t **token_list);
void		open_quotes(t_t *t, t_t **token_list, bool *free_input);
void		add_token(t_t *t, t_t **token_list);
void		initStruct(t_t *t);
void		add_token_2(t_t *new_token, t_t **token_list, int redir_control,  t_t *t);
int			alloc_new_token(t_t **new_token, int len);
void		triple_meta(t_t *t, t_t **token_list);
t_t			*set_metachar_type(t_t **token_list);
t_command	*parse(t_t *token);
t_command	*parse_commands(t_t *token);
void		parse_commands_2(t_command **current, t_command **head, t_t *token);
void		add_argument(t_command *cmd, char *arg, bool from_redir);
void 		redir_in(t_command *cmd, t_t *token);
void		redir_out(t_command *cmd, t_t *token);
void		add_pipe(t_command **head, t_command *new_node);
void print_commands(t_command *cmd); // per i test, cancellare alla fine
void 		free_command_list(t_command *cmd);
void 		free_token_list(t_t *token);
void		free_command(t_command *cmd);
bool		check_errorNclose(t_command **head, t_command *current, bool error);
void		check_pipes(t_t *t, t_t **token_list);
void    	check_pipes_2(t_t *t, t_t **token_list, size_t start, char *word);
void		add_custom_token(char *value, int type, t_t **token_list);
void		is_var(t_t *t, t_t **token_list, t_env *env);
void		is_var_2(t_t *t, t_t **token_list);
void		free_quotes(char *str1, char *str2, char *str3);
void		check_var(t_t *t);
void		new_input(t_t *t, char *exp_var, int count, int dollar);
bool		expand_exit_status(t_t *t);
void		prepare_quotes(t_t *t, t_t **token_list, bool *free_input);
void		prepare_str(t_t *t, t_t **token_list);
void		last_str(t_t *t, char *str, t_t **token_list);
void		temp_token(t_t *t, char *str);
bool		check_redirs(char pos);
void		free_command_args(t_command *cmd);
void		free_paths(char **paths);
void        free_command_l(t_command *cmd_list);
void		free_env_list(t_env *head);

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
int builtin_echo(t_command *cmd);

void	cleanup_resources(t_env *env, t_global *global);
void apply_redirections(t_command *cmd, t_env *env, t_global *global);
void	apply_redir_in1(t_redir *r, t_env *env, t_command *cmd, t_global *global);
void	apply_redir_out1(t_redir *r, t_env *env, t_command *cmd, t_global *global);
void apply_redir_out2(t_redir *r);
char *mini_getline(const char *prompt);
void create_heredoc_open(const char *delimiter, t_global *g);
void create_heredoc_effective(const char *delimiter);
void	handle_child_process(t_command *cmd, t_p_fd p_fd, t_env *env, t_global* global);
void handle_parent_process(int *prev_fd, int pipe_fd[]);
void setup_pipe(t_command *cmd, int pipe_fd[]);
void fork_process(pid_t *pid);
void wait_for_children(pid_t last_pid);
void exec_command_list(t_command *cmd_list, t_env *env, t_global *g);
char *get_command_path(char *cmd, t_env *env);
char	**convert_env_list_to_array(t_env *env);
bool is_builtin(t_command *cmd);
//void	exec_single_simple_command(t_command *cmds, t_env **env);
void	exec_builtin(t_command *cmds, t_env **env);
void	exec_single_non_builtin(t_command *cmds, t_env **env);
void	builtin_exit(t_command *cmd);

void	sigint_handler(int signum);
void	init_key_value(t_key_value *data, char *arg, char *equal_pos, int is_append);
void	handle_append_case(t_key_value *data, t_env **env);
void	update_or_add_env(t_key_value *data, t_env **env);
void	cleanup_key_value(t_key_value *data);

void redir_append(t_command *cmd, t_t *token);
void	redir_heredoc(t_command *cmd, t_t *token);

void init_shlvl(t_env **env);

t_command *init_command(void);

void apply_redir_heredoc(void);

# endif