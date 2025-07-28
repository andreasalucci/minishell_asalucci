/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 14:53:55 by inbauman          #+#    #+#             */
/*   Updated: 2024/11/27 14:53:56 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*dest;
	size_t	i;

	i = ft_strlen(s);
	if (i <= start)
	{
		dest = malloc(1);
		if (dest == NULL)
			return (NULL);
		dest [0] = '\0';
		return (dest);
	}
	if ((start + len) > i)
		len = i - start;
	dest = malloc(len +1);
	if (dest == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dest[i] = s[start + i];
		i++;
	}
	dest[len] = '\0';
	return (dest);
}
