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
        printf("minishell: syntax error near unexpected token '|'\n");
        t->error = true;
		g_exit_status = 2;
    }
    else
    {
		if(t->input[t->pos])
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

void is_var(t_t *t, t_t **token_list, t_env *env)
{
    if (t->input[t->pos] == '$' && t->input[t->pos + 1] == '?')
    {
		
        char *exit_status = ft_itoa(g_exit_status);
        t->pos += 2;
        if (ft_isalnum(t->input[t->pos]) || t->input[t->pos] == '_')
        {
            size_t start = t->pos;
            while (t->input[t->pos] && (ft_isalnum(t->input[t->pos]) || t->input[t->pos] == '_'))
                t->pos++;
            char *suffix = ft_substr(t->input, start, t->pos - start);
            char *joined = ft_strjoin(exit_status, suffix);
            add_custom_token(joined, TOKEN_WORD, token_list);
            free(suffix);
            free(joined);
        }
        else
		{
			if (t->input[t->pos] == '\'' || t->input[t->pos] == '"')
			{
				temp_token(t, exit_status);
				t->anchor_pos = t->pos;
      			return;
			}
				
			else
				add_custom_token(exit_status, TOKEN_WORD, token_list);

		} 
        free(exit_status);
        t->anchor_pos = t->pos;
        return;
    }

	
    char *var_temp;
    char *var;
    char *var_word;
    char *tmp_str;

    if (expand_exit_status(t))
        return;
    if (t->input[t->anchor_pos] == ' ')
        t->anchor_pos++;

    if (t->input[t->anchor_pos] == '$')
    {
		
        if (!t->input[t->anchor_pos + 1] || t->input[t->anchor_pos + 1] == ' ')
        {
            if (t->tmp_token)
            {
                tmp_str = malloc(2);
                if (!tmp_str)
                    return;
                tmp_str[0] = '$';
                tmp_str[1] = '\0';
                last_str(t, tmp_str, token_list);
                t->anchor_pos++;
                t->pos = t->anchor_pos;
            }
            else
            {
				
                add_custom_token("$", TOKEN_WORD, token_list);
                t->anchor_pos++;
                t->pos = t->anchor_pos;
            }
            return;
        }

        if (t->pos == t->anchor_pos)
            t->pos++;

        while (t->input[t->pos] && (ft_isalnum(t->input[t->pos]) || t->input[t->pos] == '_'))
            t->pos++;
        size_t len = t->pos - (t->anchor_pos + 1);
        var_temp = malloc(len + 1);
        if (!var_temp)
            return;
        ft_strlcpy(var_temp, t->input + t->anchor_pos + 1, len + 1);

        var = get_env_value(env, var_temp);
        if (!var)
        {
            free(var_temp);
            t->anchor_pos = t->pos;
			if (t->tmp_token)
			{
				add_custom_token(t->tmp_token, TOKEN_WORD, token_list);
				free(t->tmp_token);
				t->tmp_token = NULL;
			}
			
            return;
        }

        var_word = ft_strdup(var);
		if (t->tmp_token)
			last_str(t, var_word, token_list);
		else
		{
			add_custom_token(var_word, TOKEN_VAR, token_list);
			free(var_word);
		}		
        	
        
        free(var_temp);
        t->anchor_pos = t->pos;
    }
    else
        is_var_2(t, token_list);
}


void is_var_2(t_t *t, t_t **token_list)
{
    size_t dolar = t->pos;
    char *prefix;
    char *var;
    char *var_token;
    char *end_var;
	
    prefix = malloc(t->pos - t->anchor_pos + 1);
    if (!prefix)
        return;

    ft_strlcpy(prefix, t->input + t->anchor_pos, (t->pos - t->anchor_pos) + 1);
	//ft_printf("prefix:: %s, anchor:: %c\b", prefix, t->input[t->anchor_pos]);
    while (t->input[t->pos + 1] && (ft_isalnum(t->input[t->pos + 1]) || t->input[t->pos + 1] == '_'))
        t->pos++;

    if (t->pos != dolar)
        dolar++;

    size_t len = t->pos - dolar;
    var = malloc(len + 2);
    if (!var)
    {
        free(prefix);
        return;
    }

    ft_strlcpy(var, t->input + dolar, len + 2);
    var_token = getenv(var);
    if (!var_token)
    {
		end_var = ft_strjoin(prefix, var);
        add_custom_token(end_var, TOKEN_WORD, token_list);
        free(prefix);
        free(var);
		free(end_var);
		t->pos++;
        t->anchor_pos = t->pos;
        return;
    }
    end_var = ft_strjoin(prefix, var_token);
	
    add_custom_token(end_var, TOKEN_VAR, token_list);

    free(var);
    free(end_var);
    free(prefix);
    t->anchor_pos = t->pos;
}


