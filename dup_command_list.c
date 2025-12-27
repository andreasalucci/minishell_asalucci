#include "minishell.h"

// Funzione helper per duplicare un array di stringhe
static char	**dup_str_array(char **array)
{
	char	**new_array;
	int		count;
	int		i;

	if (!array)
		return (NULL);
	count = 0;
	while (array[count])
		count++;
	new_array = malloc(sizeof(char *) * (count + 1));
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < count)
	{
		if (array[i])
			new_array[i] = strdup(array[i]);
		else
			new_array[i] = NULL;
		if (array[i] && !new_array[i])
		{
			while (--i >= 0)
				free(new_array[i]);
			free(new_array);
			return (NULL);
		}
		i++;
	}
	new_array[count] = NULL;
	return (new_array);
}

// Funzione helper per duplicare un array di bool
static bool	*dup_bool_array(bool *array, int size)
{
	bool	*new_array;
	int		i;

	if (!array || size <= 0)
		return (NULL);
	new_array = malloc(sizeof(bool) * size);
	if (!new_array)
		return (NULL);
	i = 0;
	while (i < size)
	{
		new_array[i] = array[i];
		i++;
	}
	return (new_array);
}

// Funzione helper per duplicare la lista di redirezioni
static t_redir	*dup_redir_list(t_redir *redirs)
{
	t_redir	*new_list;
	t_redir	*new_node;
	t_redir	*current;
	t_redir	*prev;

	if (!redirs)
		return (NULL);
	new_list = NULL;
	prev = NULL;
	current = redirs;
	while (current)
	{
		new_node = malloc(sizeof(t_redir));
		if (!new_node)
			return (NULL);
		new_node->type = current->type;
		if (current->filename)
			new_node->filename = strdup(current->filename);
		else
			new_node->filename = NULL;
		new_node->next = NULL;
		if (current->filename && !new_node->filename)
		{
			free(new_node);
			// Cleanup della lista parziale (da fare)
			return (NULL);
		}
		if (prev)
			prev->next = new_node;
		else
			new_list = new_node;
		prev = new_node;
		current = current->next;
	}
	return (new_list);
}

// Funzione per duplicare un singolo nodo t_command
static t_command	*dup_command_node(t_command *src)
{
	t_command	*dst;
	int			argc;

	if (!src)
		return (NULL);
	dst = malloc(sizeof(t_command));
	if (!dst)
		return (NULL);
	// Inizializza i campi
	dst->argv = NULL;
	dst->arg_is_redir = NULL;
	dst->infile = NULL;
	dst->outfile = NULL;
	dst->redirs = NULL;
	dst->next = NULL;
	dst->in_hdc = src->in_hdc;
	// Duplica argv e arg_is_redir
	if (src->argv)
	{
		dst->argv = dup_str_array(src->argv);
		if (!dst->argv)
		{
			free(dst);
			return (NULL);
		}
		// Calcola argc per duplicare arg_is_redir
		argc = 0;
		while (src->argv[argc])
			argc++;
		if (src->arg_is_redir && argc > 0)
		{
			dst->arg_is_redir = dup_bool_array(src->arg_is_redir, argc);
			if (!dst->arg_is_redir)
			{
				// Cleanup
				for (int i = 0; dst->argv[i]; i++)
					free(dst->argv[i]);
				free(dst->argv);
				free(dst);
				return (NULL);
			}
		}
	}
	// Duplica infile e outfile
	if (src->infile)
	{
		dst->infile = strdup(src->infile);
		if (!dst->infile)
		{
			// Cleanup
			if (dst->argv)
			{
				for (int i = 0; dst->argv[i]; i++)
					free(dst->argv[i]);
				free(dst->argv);
			}
			free(dst->arg_is_redir);
			free(dst);
			return (NULL);
		}
	}
	if (src->outfile)
	{
		dst->outfile = strdup(src->outfile);
		if (!dst->outfile)
		{
			// Cleanup
			if (dst->argv)
			{
				for (int i = 0; dst->argv[i]; i++)
					free(dst->argv[i]);
				free(dst->argv);
			}
			free(dst->arg_is_redir);
			free(dst->infile);
			free(dst);
			return (NULL);
		}
	}
	// Duplica la lista di redirezioni
	if (src->redirs)
	{
		dst->redirs = dup_redir_list(src->redirs);
		if (!dst->redirs)
		{
			// Cleanup completo
			if (dst->argv)
			{
				for (int i = 0; dst->argv[i]; i++)
					free(dst->argv[i]);
				free(dst->argv);
			}
			free(dst->arg_is_redir);
			free(dst->infile);
			free(dst->outfile);
			free(dst);
			return (NULL);
		}
	}
	return (dst);
}

// Funzione principale per duplicare l'intera lista di comandi
t_command	*dup_command_list(t_command *src_list)
{
	t_command	*dst_list;
	t_command	*dst_current;
	t_command	*dst_prev;
	t_command	*src_current;

	if (!src_list)
		return (NULL);
	dst_list = NULL;
	dst_prev = NULL;
	src_current = src_list;
	while (src_current)
	{
		dst_current = dup_command_node(src_current);
		if (!dst_current)
		{
			// Cleanup dell'intera lista duplicata finora
			// (richiede una funzione free_command_list separata)
			return (NULL);
		}
		if (dst_prev)
			dst_prev->next = dst_current;
		else
			dst_list = dst_current;
		dst_prev = dst_current;
		src_current = src_current->next;
	}
	return (dst_list);
}