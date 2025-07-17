#ifndef MINISHELL_H

#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
#include <stdlib.h>



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
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	bool			single_quote;
	bool			double_quote;
	char			*input;
	char			*start;
	size_t			pos;
	size_t			anchor_pos;
	bool			error;
} t_t;

typedef struct s_command 
{
    char				**argv;        
    char				*infile;       
    char				*outfile;      
    int					redir_in;      
    int					redir_out;     
    struct s_command	*next;
} t_command;

t_t	*tokens(char *input);
void		quotes(t_t *t);
void		metacharacters(t_t *t, t_t **token_list);
void		open_quotes(t_t *t, t_t **token_list);
void		add_token(t_t *t, t_t **token_list);
void		initStruct(t_t *t);
void		add_token_2(t_t *new_token, t_t **token_list);
int			alloc_new_token(t_t **new_token, int len);
void		triple_meta(t_t *t, t_t **token_list);
t_t			*set_metachar_type(t_t **token_list);
void		parse(t_t *token);
t_command	*parse_commands(t_t *token);
void		parse_commands_2(t_command **current, t_command **head, t_t *token);
void		add_argument(t_command *cmd, char *arg);
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
void		is_var(t_t *t, t_t **token_list);
void		is_var_2(t_t *t, t_t **token_list);
# endif