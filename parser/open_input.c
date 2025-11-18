#include "../minishell.h"

bool	only_spaces_after_pipe(char *pp)
{
	size_t	i;

	i = 1;
	while (pp[i])
	{
		if (!(pp[i] == ' ' || pp[i] == '\t'))
			return (0);
		i++;
	}
	return (1);
}

bool	quotes_closed(char *input)
{
	bool	in_single;
	bool	in_double;
	char	*p;

	in_single = false;
	in_double = false;
	p = input;
	while (*p)
	{
		if (*p == '\'' && !in_double)
			in_single = !in_single;
		else if (*p == '"' && !in_single)
			in_double = !in_double;
		else if (*p == '\\')
			if (*(p + 1))
				p++;
		p++;
	}
	return (!in_single && !in_double);
}

bool	input_is_open(char *input)
{
	char	*pp;

	if (!quotes_closed(input))
	{
		ft_putstr_fd("minishell: Syntax error: unclosed quotes\n", 2);
		//free(input);
		//input = NULL;
		g_exit_status = 2;
		return (true);
	}
	pp = ft_strchr(input, '|');
	if (pp && only_spaces_after_pipe(pp))
	{
		ft_putstr_fd("minishell: Syntax error: unspecified pipe\n", 2);
		//free(input);
		//input = NULL;
		g_exit_status = 2;
		return (true);
	}
	return (false);
}
