#include "../minishell.h"

char	*ret_read_hdc_line(int bytes, int i, char *buffer)
{
	if (bytes <= 0 && i == 0)
	{
		write(STDOUT_FILENO, "\n", 1);
		return (NULL);
	}
	return (ft_strdup(buffer));
}

char	*read_hdc_line(bool *hdc_interrupted)
{
	char	buffer[1024];
	int		i;
	int		bytes;
	char	ch;

	i = 0;
	write(STDOUT_FILENO, "> ", 2);
	while (i < 1023)
	{
		bytes = read(STDIN_FILENO, &ch, 1);
		if (bytes == -1)
		{
			if (errno == EINTR)
			{
				*hdc_interrupted = true;
				return (NULL);
			}
		}
		if (bytes == 0 || ch == '\n')
			break ;
		buffer[i++] = ch;
	}
	buffer[i] = '\0';
	return (ret_read_hdc_line(bytes, i, buffer));
}
