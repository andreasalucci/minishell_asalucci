#include "../minishell.h"

int	alloc_new_token(t_t **new_token, int len)
{
	*new_token = malloc(sizeof(t_t));
	if (!*new_token)
		return (0);
	(*new_token)->value = malloc(len + 2);
	if (!(*new_token)->value)
	{
		free((*new_token)->value);
		free(*new_token);
		return (0);
	}
	return (1);
}

void	free_token_list(t_t *token)
{
	t_t	*tmp;

	while (token)
	{
		tmp = token->next;
		free(token->value);
		free(token);
		token = tmp;
	}
}

void	free_arrays_array(char **arrays_array)
{
	int	i;

	i = 0;
	while (arrays_array[i])
	{
		free(arrays_array[i]);
		i++;
	}
	free(arrays_array);
}

void	free_command_list(t_command *cmd)
{
	t_command	*tmp;
	int			i;

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
