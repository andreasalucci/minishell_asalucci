/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:08:14 by asalucci          #+#    #+#             */
/*   Updated: 2025/01/10 16:08:16 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <fcntl.h>

char	*get_next_line_4(char *constr, char **remainings)
{
	int			end;
	char		*constrn;
	char		*newline_pos;

	newline_pos = ft_strchr(constr, '\n');
	end = 0;
	if (newline_pos)
	{
		*remainings = ft_strdup(newline_pos + 1);
		*newline_pos = '\0';
	}
	else
	{
		end = 1;
		*remainings = NULL;
	}
	if (end)
		return (constr);
	constrn = f_constrn(constr);
	free(constr);
	return (constrn);
}

char	*get_next_line_3(char *constr, int bytes_read, char **remainings)
{
	if (bytes_read <= 0 && !*constr)
	{
		free(constr);
		return (NULL);
	}
	constr = get_next_line_4(constr, remainings);
	return (constr);
}

char	*get_next_line_2(int fd, char *constr, char *buf, char **remainings)
{
	int			bytes_read;
	char		*temp;

	bytes_read = 1;
	while (!ft_strchr(constr, '\n') && bytes_read > 0)
	{
		bytes_read = read(fd, buf, BUFFER_SIZE);
		if (bytes_read < 0)
		{
			free(buf);
			free(constr);
			return (NULL);
		}
		buf[bytes_read] = '\0';
		temp = ft_strjoin(constr, buf);
		free(constr);
		constr = temp;
	}
	free(buf);
	constr = get_next_line_3(constr, bytes_read, remainings);
	return (constr);
}

char	*get_next_line(int fd)
{
	char		*buf;
	char		*constr;
	static char	*remainings;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (remainings)
	{
		constr = ft_strdup(remainings);
		free(remainings);
		remainings = NULL;
	}
	else
		constr = ft_strdup("");
	buf = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buf)
	{
		free(constr);
		return (NULL);
	}
	constr = get_next_line_2(fd, constr, buf, &remainings);
	return (constr);
}
/*
int main()
{
	//cd 0_HolyGraph/CC_0perativa/get_next_line
	//cc -g get_next_line.c get_next_line_utils.c
	//valgrind --leak-check=full --show-leak-kinds
	//=all --track-origins=yes ./a.out
	//cc get_next_line.c -D BUFFER_SIZE=200 get_next_line_utils.c

    int     fd = open("file", O_RDONLY);
    char    *line;
	int i = 0;
	while(i < 100)
	{
		line = get_next_line(fd);
		if(!line)
		{
			printf("NULL");
			free(line);
			break;
		}
		printf("[%s]", line);
		i++;
		if (line)
			free(line);
	}
	close(fd);
    return (0);
}*/