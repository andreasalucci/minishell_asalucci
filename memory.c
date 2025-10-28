#include "minishell.h"
#include "libft.h"

int alloc_new_token(t_t **new_token, int len)
{
	*new_token = malloc(sizeof(t_t));
	if (!*new_token)
		return (0) ;
	(*new_token)->value = malloc(len +2);
	if (!(*new_token)->value)
	{
		free((*new_token)->value);
		free(*new_token);
		return (0);
	}
	return (1);
}

void free_command_list(t_command *cmd)
{
	t_command *tmp;
	int i;

	while (cmd)
	{
		i = 0;
		tmp = cmd->next;
		if (cmd->argv)
		{
			while (cmd->argv[i])
				free(cmd->argv[i++]);
			free(cmd->argv);
		}
		free(cmd->infile);
		free(cmd->outfile);
		free(cmd);
		cmd = tmp;
	}
}

void free_token_list(t_t *token)
{
	t_t *tmp;

	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token);
		token = tmp;
	}
}

void free_command(t_command *cmd)
{
	int i = 0;
	t_redir *r;
	t_redir *tmp;

	if (!cmd)
		return;
	if (cmd->arg_is_redir)
	{
		free(cmd->arg_is_redir);
		 cmd->arg_is_redir = NULL;
	}
	if (cmd->argv)
	{
		while (cmd->argv[i])
			free(cmd->argv[i++]);
		free(cmd->argv);
	}
	  // ← Añadido
	r = cmd->redirs;
	while (r)
	{
		tmp = r->next;
		free(r->filename);
		free(r);
		r = tmp;
	}
	free(cmd->infile);
	free(cmd->outfile);
	free(cmd);
}

void free_paths(char **paths)
{
	int i;

	i = 0;
	while(paths[i])
	{
		free(paths[i]);
		i++;
	}
	free(paths);
}

void free_command_l(t_command *cmd_list)
{
	t_command	*tmp;

	while (cmd_list)
	{
		tmp = cmd_list->next;
		free_command(cmd_list);
		cmd_list = tmp;
	}
}
