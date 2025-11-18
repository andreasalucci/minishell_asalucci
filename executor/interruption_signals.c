#include "../minishell.h"

void	setup_shell_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	sigint_handler(int signum)
{
	(void)signum;
	g_exit_status = 130;
	if (isatty(STDIN_FILENO))
	{
		write(STDOUT_FILENO, "\n", 1);
		if (!rl_done)
		{
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
	}
}

void	heredoc_sigint_handler(int signum)
{
	(void)signum;
	write(STDOUT_FILENO, "\n", 1);
	g_exit_status = 130;
	close(STDIN_FILENO);
}

int	handle_eof(char *input)
{
	if (!input)
	{
		free(input);
		input = NULL;
		return (1);
	}
	return (0);
}

int	handle_input_interruption(bool *hdc_interrupted, char *input)
{
	if (*hdc_interrupted)
	{
		*hdc_interrupted = false;
		free(input);
		input = NULL;
		return (1);
	}
	return (0);
}
