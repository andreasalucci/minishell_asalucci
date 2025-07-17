/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:31:55 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 17:32:06 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>

/*
La función ft_split toma una cadena y un delimitador, 
divide la cadena en subcadenas según el delimitador,
y devuelve un array de punteros a esas subcadenas, 
con un puntero nulo (NULL) al final del array para 
indicar el final de las subcadenas.
*/

static int	coincidencias(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (*s == '\0')
		return (0);
	if (s[i] != c && s[i] != '\0')
		count++;
	while (s[i] != '\0')
	{
		if (s[i] == c && s[i + 1] != c && s[i + 1] != '\0')
			count++;
		i++;
	}
	return (count);
}

static int	word_length(char const *s, char c)
{
	int	len;

	len = 0;
	while (s[len] != c && s[len] != '\0')
		len++;
	return (len);
}

static char	**free_memory(char **result, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(result[i]);
		i++;
	}
	free(result);
	return (NULL);
}

static char	*allocate_word(char const *s, char c, int *i)
{
	int		j;
	int		len;
	char	*word;

	len = word_length(&s[*i], c);
	word = malloc((len + 1) * sizeof(char));
	if (!word)
		return (NULL);
	j = 0;
	while (s[*i] != c && s[*i] != '\0')
		word[j++] = s[(*i)++];
	word[j] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;
	int		k;

	if (!s)
		return (NULL);
	result = malloc((coincidencias(s, c) + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	i = 0;
	k = 0;
	while (s[i] != '\0')
	{
		while (s[i] == c)
			i++;
		if (s[i] != '\0')
		{
			result[k] = allocate_word(s, c, &i);
			if (!result[k])
				return (free_memory(result, k));
			k++;
		}
	}
	result[k] = NULL;
	return (result);
}
