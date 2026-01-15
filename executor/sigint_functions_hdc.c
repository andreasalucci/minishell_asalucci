#include "../minishell.h"

void	sigint_heredoc(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
}

void	set_sigint_heredoc(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

int	handle_input_interruption(bool *hdc_interrupted, char *input)
{
	if (*hdc_interrupted)
	{
		*hdc_interrupted = false;
		free(input);
		return (1);
	}
	return (0);
}
