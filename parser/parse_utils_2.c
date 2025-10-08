#include "../minishell.h"
#include "../libft/libft.h"

void add_pipe(t_command **head, t_command *new_node)
{
	
	t_command *tmp;

	if (!head || !new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new_node;

}

bool	check_errorNclose(t_command **head, t_command *current, bool error)
{
	if (error)
	{
		free_command_list(*head);
		if (current)
			free_command(current);
	}
	return (error);
}

char *extract_quoted_string(const char *src, int *i)
{
    char quote = src[*i]; // può essere ' o "
    int start = ++(*i);   // salta la prima virgoletta
    int len = 0;

    while (src[*i] && src[*i] != quote) {
        (*i)++;
        len++;
    }

    char *result = malloc(len + 1);
    if (!result) return NULL;
    strncpy(result, src + start, len);
    result[len] = '\0';

    if (src[*i] == quote) // salta la virgoletta finale
        (*i)++;

    return result;
}