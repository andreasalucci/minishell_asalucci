#include "minishell.h"
#include "libft.h"


int	main(int argc, char *argv[], char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	char *input;
	t_t *token;

	while(1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			ft_printf("exit\n");
			break ;
		}
		if (*input != '\0')
			add_history(input);
		token = tokens(input);
		if (token)
    		parse(token);
		free(input);
	}
}
