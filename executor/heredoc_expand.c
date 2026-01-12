#include "../minishell.h"

void	if_expand(t_heredoc *hdc, t_hdc_context *ctx, char	**line)
{
	char	*expanded;

	if (hdc->expand)
	{
		expanded = expand_heredoc_line(*line, ctx->env);
		if (expanded)
		{
			write(hdc->fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
	}
	else
	{
		write(hdc->fd, *line, ft_strlen(*line));
		free(*line);
	}
}

void	expand_hdc_exit_status(char **var_value, char **result, int *i)
{
	*var_value = ft_itoa(g_exit_status);
	*result = ft_strjoin_free(*result, *var_value);
	free(*var_value);
	*i += 2;
}

void	expand_hdc_env_var(int *i, char **line, char **result, t_env *env)
{
	int		start;
	char	*var_name;
	char	*var_value;

	start = *i + 1;
	while ((*line)[start] && (ft_isalnum((*line)[start])
		|| (*line)[start] == '_'))
		(start)++;
	var_name = ft_substr(*line, *i + 1, start - *i - 1);
	var_value = get_env_value(env, var_name);
	if (var_value)
		*result = ft_strjoin_free(*result, var_value);
	free(var_name);
	*i = start;
}

void	hdc_line_create_normal(char **line, char **result, int *i)
{
	char	temp[2];

	temp[0] = (*line)[*i];
	temp[1] = '\0';
	*result = ft_strjoin_free(*result, temp);
	(*i)++;
}

char	*expand_heredoc_line(char *line, t_env *env)
{
	char	*result;
	char	*var_value;
	int		i;

	if (!line)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	while (line[i])
	{
		if (line[i] == '$' && (ft_isalpha(line[i + 1])
				|| line[i + 1] == '_' || line[i + 1] == '?'))
		{
			if (line[i + 1] == '?')
			{
				expand_hdc_exit_status(&var_value, &result, &i);
				continue ;
			}
			expand_hdc_env_var(&i, &line, &result, env);
		}
		else
			hdc_line_create_normal(&line, &result, &i);
	}
	return (free(line), result);
}
