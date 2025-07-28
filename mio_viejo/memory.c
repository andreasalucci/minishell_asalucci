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