#include "../minishell.h"

int	process_cmd_heredocs(t_command *cmd, t_hdc_context *ctx)
{
	t_redir	*r;
	int		result;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == REDIR_HEREDOC)
		{
			cmd->in_hdc = true;
			result = process_heredoc(r->filename, cmd, *ctx);
			if (result != 0)
				return (result);
			(*ctx->counter)++;
		}
		r = r->next;
	}
	return (0);
}

bool	has_heredoc_redir(t_redir *r)
{
	while (r)
	{
		if (r->type == REDIR_HEREDOC)
			return (true);
		r = r->next;
	}
	return (false);
}

void	init_ctx(int *counter, t_hdc_context *ctx, t_env *env,
					bool *hdc_interrupted)
{
	(*ctx).env = env;
	(*ctx).interrupted = hdc_interrupted;
	(*ctx).counter = counter;
}

int	process_all_heredocs(t_command *cmd_list, t_env *env,
							bool *hdc_interrupted)
{
	t_command		*cmd;
	t_hdc_context	ctx;
	int				result;
	int				counter;

	set_sigint_heredoc();
	cmd = cmd_list;
	counter = 0;
	init_ctx(&counter, &ctx, env, hdc_interrupted);
	while (cmd)
	{
		if (has_heredoc_redir(cmd->redirs))
		{
			result = process_cmd_heredocs(cmd, &ctx);
			if (result == 130)
			{
				g_exit_status = 130;
				return (cleanup_heredoc_files(cmd_list), 130);
			}
			if (result < 0)
				return (cleanup_heredoc_files(cmd_list), -1);
		}
		cmd = cmd->next;
	}
	return (0);
}

void	cleanup_heredoc_files(t_command *cmd_list)
{
	t_command	*cmd;
	t_redir		*r;

	cmd = cmd_list;
	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->filename && ft_strnstr(r->filename, "minish_hd_",
					ft_strlen(r->filename)) != NULL)
				unlink(r->filename);
			r = r->next;
		}
		cmd = cmd->next;
	}
}
