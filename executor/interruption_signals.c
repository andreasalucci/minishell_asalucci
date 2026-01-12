#include "../minishell.h"

void	sigint_main(int signum)
{
	(void)signum;
	g_exit_status = 130;
	if (isatty(STDIN_FILENO))
	{
		if (rl_readline_state & RL_STATE_READCMD)
		{
			write(STDOUT_FILENO, "\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			if (rl_line_buffer)
				rl_redisplay();
		}
		else
			write(STDOUT_FILENO, "\n", 1);
	}
}

void	set_sigint_main(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_main;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
}

void	sigint_executing(int signum)
{
	(void)signum;
	g_exit_status = 130;
	write(0, "\n", 1);
}

void	set_sigint_executing(void)
{
	struct sigaction	sa;

	sa.sa_handler = sigint_executing;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
}

int	handle_eof(char *input)
{
	if (!input)
		return (1);
	return (0);
}
