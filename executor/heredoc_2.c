#include "../minishell.h"

int	process_hdc_line(t_heredoc *hdc, t_hdc_context *ctx)
{
	char	*line;

	line = read_hdc_line(ctx->interrupted);
	if (!line || ft_strcmp(line, hdc->clean_delim) == 0)
	{
		free(line);
		if (*ctx->interrupted)
		{
			unlink(hdc->tmp_file);
			free(hdc->tmp_file);
			free(hdc->clean_delim);
			return (130);
		}
		return (1);
	}
	if_expand(hdc, ctx, &line);
	write(hdc->fd, "\n", 1);
	return (0);
}

int	read_heredoc_input(t_heredoc *hdc, t_hdc_context *ctx)
{
	int	result;

	while (!*ctx->interrupted)
	{
		result = process_hdc_line(hdc, ctx);
		if (result == 130)
			return (130);
		if (result == 1)
			break ;
	}
	return (0);
}

void	apply_redir_hdc(t_heredoc *hdc, t_command *cmd)
{
	t_redir	*r;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			r->type = REDIR_IN;
			if (r->filename)
				free(r->filename);
			r->filename = hdc->tmp_file;
			hdc->tmp_file = NULL;
			break ;
		}
		r = r->next;
	}
}

int	create_hdc_file(t_heredoc *hdc, t_hdc_context *ctx)
{
	char	*num;

	hdc->tmp_file = ft_strdup("./minish_hd_");
	num = ft_itoa((*ctx->counter)++);
	hdc->tmp_file = ft_strjoin_free(hdc->tmp_file, num);
	free(num);
	hdc->fd = open(hdc->tmp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (hdc->fd < 0)
	{
		perror("minishell");
		free(hdc->tmp_file);
		free(hdc->clean_delim);
		return (-1);
	}
	return (0);
}

int	process_heredoc(const char *delimiter, t_command *cmd,
					t_hdc_context ctx)
{
	t_heredoc	hdc;
	int			result;

	hdc.expand = 1;
	if (delimiter[0] == '\'' || delimiter[0] == '"')
		hdc.expand = 0;
	if (delimiter[0] == '\'' || delimiter[0] == '"')
		hdc.clean_delim = ft_strtrim(delimiter, "\"'");
	else
		hdc.clean_delim = ft_strdup(delimiter);
	result = create_hdc_file(&hdc, &ctx);
	if (result < 0)
		return (result);
	result = read_heredoc_input(&hdc, &ctx);
	if (result != 0)
		return (result);
	close(hdc.fd);
	if (!*ctx.interrupted)
		apply_redir_hdc(&hdc, cmd);
	free(hdc.clean_delim);
	return (0);
}
