#include "../minishell.h"

// Signal handler per heredoc senza fork
// void	heredoc_signal_no_fork(int sig)
// {
// 	(void)sig;
// 	g_hd_interrupt = 1;
// 	write(STDOUT_FILENO, "\n", 1);
// }

// Legge una linea per heredoc senza readline
static char	*read_heredoc_line_simple(bool *hdc_interrupted)
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
				return NULL;   // SIGINT → esci heredoc
			}
		}
		if (bytes == 0 || ch == '\n')
			break;
		buffer[i++] = ch;
	}
	buffer[i] = '\0';
	
	if (bytes <= 0 && i == 0) // EOF (Ctrl+D)
		return (NULL);
	
	return (ft_strdup(buffer));
}

// Espande variabili in linea heredoc
static char	*expand_heredoc_line(char *line, t_env *env)
{
	char	*result;
	char	temp[2];
	char	*var_value;
	int		i;
	int		start;

	if (!line)
		return (NULL);
	
	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && (ft_isalpha(line[i + 1]) || 
			line[i + 1] == '_' || line[i + 1] == '?'))
		{
			if (line[i + 1] == '?')
			{
				var_value = ft_itoa(g_exit_status);
				result = ft_strjoin_free(result, var_value);
				free(var_value);
				i += 2;
				continue;
			}
			
			start = i + 1;
			while (line[start] && (ft_isalnum(line[start]) || line[start] == '_'))
				start++;
			
			char *var_name = ft_substr(line, i + 1, start - i - 1);
			var_value = get_env_value(env, var_name);
			if (var_value)
			{
				result = ft_strjoin_free(result, var_value);
				//free(var_value);
			}
			free(var_name);
			i = start;
		}
		else
		{
			temp[0] = line[i];
			temp[1] = '\0';
			result = ft_strjoin_free(result, temp);
			i++;
		}
	}
	free(line);
	return (result);
}

// Processa heredoc SENZA FORK
int	process_heredoc(const char *delimiter, t_command *cmd, 
				t_env *env, bool *hdc_interrupted)
{
	int		fd;
	char	*line;
	char	*clean_delim;
	int		expand;
	char	*tmp_fileSSS;
	char	*tmp_file;
	static int counter = 0;

	// Determina se espandere
	expand = 1;
	if (delimiter[0] == '\'' || delimiter[0] == '"')
		expand = 0;
	
	// Pulisci delimitatore
	if (delimiter[0] == '\'' || delimiter[0] == '"')
		clean_delim = ft_strtrim(delimiter, "\"'");
	else
		clean_delim = ft_strdup(delimiter);
	
	// Crea file temporaneo
	tmp_fileSSS = ft_strdup("./minish_hd_");
	char *num = ft_itoa(counter++);
	tmp_file = ft_strjoin_free(tmp_fileSSS, num);
	free(num);

	fd = open(tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror("minishell");
					free(tmp_file);
		free(clean_delim);
		return (-1);
	}

	// Leggi input
	while (!*hdc_interrupted)
	{
		line = read_heredoc_line_simple(hdc_interrupted);
		if (!line || ft_strcmp(line, clean_delim) == 0)
		{
			free(line);
			if (*hdc_interrupted)
			{
				unlink(tmp_file);
				free(tmp_file);
				free(clean_delim);
				return (130);
			}
			break ;
		}
		if (expand)
		{
			char *expanded = expand_heredoc_line(line, env);
			if (expanded)
			{
				write(fd, expanded, ft_strlen(expanded));
				free(expanded);
			}
		}
		else
		{
			write(fd, line, ft_strlen(line));
			free(line);
		}
		write(fd, "\n", 1);
	}
	close(fd);


	// Sostituisci redir se non interrupted
	if (!*hdc_interrupted)
	{
		t_redir *r = cmd->redirs;
		while (r)
		{
			if (r->type == REDIR_HEREDOC)
			{
				r->type = REDIR_IN;
				if (r->filename)
					free(r->filename);
				r->filename = tmp_file;
				tmp_file = NULL;
				break;
			}
			r = r->next;
		}
	}
	
	free(clean_delim);
	return (0);
}

