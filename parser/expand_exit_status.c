#include "../minishell.h"

bool	expand_exit_status(t_t *t)
{
	char	*before;
	char	*after;
	char	*status_str;
	char	*temp;
	char	*pos;

	pos = ft_strnstr(t->input + t->anchor_pos, "$?", ft_strlen(t->input
				+ t->anchor_pos));
	if (pos)
	{
		before = ft_substr(t->input, 0, pos - t->input);
		after = ft_strdup(pos + 2);
		status_str = ft_itoa(g_exit_status);
		temp = ft_strjoin(before, status_str);
		free(t->input);
		t->input = ft_strjoin(temp, after);
		free(before);
		free(after);
		free(temp);
		free(status_str);
		return (1);
	}
	else
		return (0);
}
