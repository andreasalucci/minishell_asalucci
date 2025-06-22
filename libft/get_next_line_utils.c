/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 16:10:23 by asalucci          #+#    #+#             */
/*   Updated: 2025/01/10 16:10:24 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/*
size_t	ft_strlen(const char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
		i ++;
	return (i);
}

char	*ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == (unsigned char)c)
			return (((char *)&(s[i])));
		i++;
	}
	if (s[i] == (unsigned char)c)
		return ((char *)(&(s[i])));
	else
		return (NULL);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*js;
	int		i;
	int		j;

	js = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!js)
	{
		free(js);
		return (NULL);
	}
	i = 0;
	while (s1[i] != '\0')
	{
		js[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j] != '\0')
	{
		js[i + j] = s2[j];
		j++;
	}
	js[i + j] = '\0';
	return (js);
}

char	*ft_strdup(const char *s)
{
	int		i;
	char	*ds;

	i = 0;
	while (s[i] != '\0')
		i++;
	ds = (char *)malloc((sizeof (char)) * (i + 1));
	if (!ds)
	{
		free(ds);
		return (NULL);
	}
	i = 0;
	while (s[i] != '\0')
	{
		ds[i] = s[i];
		i++;
	}
	ds[i] = '\0';
	return (ds);
}
*/
char	*f_constrn(char *constr)
{
	char	*constrn;
	size_t	i;

	i = 0;
	constrn = (char *)malloc(sizeof(char) * ft_strlen(constr) + 2);
	if (!constrn)
	{
		free(constrn);
		return (NULL);
	}
	while (constr[i] != '\0')
	{
		constrn[i] = constr[i];
		i++;
	}
	constrn[i] = '\n';
	constrn[i + 1] = '\0';
	return (constrn);
}
