#include "../minishell.h"
#include "../libft/libft.h"

void check_pipes_2(t_t *t, t_t **token_list, size_t start, char *word)
{
	size_t i;

	i = 0;
    if (start < t->pos)
    {
        word = ft_substr(t->input, start, t->pos - start);
        add_custom_token(word, TOKEN_WORD, token_list);
        free(word);
    }
    t->anchor_pos = t->pos;
    i = t->pos + 1;
    while (t->input[i] == ' ')
        i++;

    if (t->pos == 0 || t->input[i] == '\0' || t->input[i] == '|')
    {
        ft_printf("minishell: syntax error near unexpected token '|'\n");
        t->error = true;
    }
    else
    {
        t->pos++;
        add_token(t, token_list);
    }
}



void    add_custom_token(char *value, int type, t_t **token_list)
{
	t_t *new_token;

	new_token = malloc(sizeof(t_t));
	if (!new_token)
		return ;

	new_token->value = ft_strdup(value);
	new_token->type = type;
	new_token->error = false;
	new_token->next = NULL;
	if (!*token_list)
		*token_list = new_token;
	else
	{
		t_t *tmp = *token_list;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_token;
	}
}
