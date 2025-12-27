#include "../minishell.h"

void	set_sigint_main(void)
{
	struct sigaction sa;

	sa.sa_handler = sigint_main;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;   // readline OK
	sigaction(SIGINT, &sa, NULL);
}

void	set_sigint_heredoc(void)//prendere l'hdc_iterrupted   e restare in attesa da altre funz'
{
	struct sigaction sa;

	sa.sa_handler = sigint_heredoc;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;            // ⬅️ NO SA_RESTART
	sigaction(SIGINT, &sa, NULL);
}

void	set_sigint_executing(void)
{
	struct sigaction sa;

	sa.sa_handler = sigint_executing;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;  // NO SA_RESTART durante l'esecuzione
	sigaction(SIGINT, &sa, NULL);
}

void	sigint_executing(int signum)
{
	(void)signum;
	g_exit_status = 130;
	// Solo nuova linea, senza ridisegnare il prompt
	write(0, "\n", 1);
}

// void	sigint_main(int signum)
// {
// 	(void)signum;
// 	g_exit_status = 130;
// 	if (isatty(0) && 0)
// 	{
// 		write(0, "\n", 1);
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 		rl_redisplay();
// 	}
// 	else if (isatty(0) && 1)
// 		write(0, "\n", 1);
// }

void sigint_main(int signum)
{
    (void)signum;
    
    g_exit_status = 130;
    
    if (isatty(STDIN_FILENO))
    {
        // Controlla se readline è attivo
        // RL_STATE_READCMD indica che readline sta leggendo input
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
    // else
    // {
    //     // Non in terminale interattivo
    //     write(STDOUT_FILENO, "\n", 1);
    // }
}

void	sigint_heredoc(int signum)
{
	(void)signum;
	//g_exit_status = 130;
	write(STDOUT_FILENO, "\n", 1);
}

int	handle_eof(char *input)
{
	if (!input)
		return (1);
	return (0);
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
