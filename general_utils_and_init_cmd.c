#include "minishell.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->argv = NULL;
	cmd->arg_is_redir = NULL;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	cmd->in_hdc = false;
	return (cmd);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

char	*ft_strjoin_3(const char *s1, const char *s2, const char *s3)
{
	size_t	len1;
	size_t	len2;
	size_t	len3;
	char	*result;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = malloc(len1 + len2 + len3 + 1);
	if (!result)
		return (NULL);
	ft_strcpy(result, s1);
	ft_strlcat(result, s2, len1 + len2 + len3 + 1);
	ft_strlcat(result, s3, len1 + len2 + len3 + 1);
	return (result);
}
