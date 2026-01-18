#include "../minishell.h"

t_command	*parse(t_t *token)
{
	t_command	*cmd_list;

	cmd_list = NULL;
	token->error = false;
	cmd_list = parse_commands(token);
	if (!cmd_list)
		return (NULL);
	if (!cmd_list->argv)
		cmd_list->argv = NULL;
	return (cmd_list);
}

t_command	*parse_input_to_commands(char *input, bool *free_input, t_env *env)
{
	t_t			*token;
	t_command	*cmd;

	token = tokens(input, free_input, env);
	if (!token)
		return (NULL);
	cmd = parse(token);
	free_token_list(token);
	return (cmd);
}
