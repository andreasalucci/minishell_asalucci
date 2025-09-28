#include "../minishell.h"

void    check_var(t_t *t)
{
	int     count;
	char    *var;
	char    *exp_var;
	int     dollar;
	
	count = t->pos;
	while(t->input[count] && t->input[count] != '"')
	{
		if (t->input[count] == '$' && t->input[count +1] == '?')
		{
			expand_exit_status(t);
		}
		else if (t->input[count] == '$' && t->input[count +1] == '"')
			return ;
		else if (t->input[count] == '$' && t->input[count +1] != ' ')
		{
			count++;
			dollar = count;
			while (t->input[count] && (ft_isalnum(t->input[count]) || t->input[count] == '_'))
				count++;
			var = malloc((count - dollar) +1);
			ft_strlcpy(var, t->input + dollar, (count - dollar) +1);
			exp_var = getenv(var);
			free(var);
			new_input(t, exp_var, count, dollar);
			
		}
		count++;
		
	}	
}

void    new_input(t_t *t, char *exp_var, int count, int dollar)
{
	char    *begin_var;
	char    *with_var;
	char    *new_input;
	char    *after_var;

	begin_var = malloc(count);
	after_var = malloc((ft_strlen(t->input) - count) +1);
	ft_strlcpy(begin_var, t->input, dollar);
	with_var = ft_strjoin(begin_var, exp_var);
	free(begin_var);
	ft_strlcpy(after_var, t->input + count, (ft_strlen(t->input) - count) +1);
	new_input = ft_strjoin(with_var, after_var);
	t->input = NULL;
	t->input = new_input;
	t->start = new_input;
}

