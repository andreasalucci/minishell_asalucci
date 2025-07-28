/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:49:23 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 17:50:23 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>
#include <stdlib.h>
#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*dest;
	size_t	i;
	size_t	n;

	if (!s1 || !set)
		return (NULL);
	n = 0;
	i = ft_strlen(s1);
	while (s1[n] && ft_strchr(set, s1[n]))
		n++;
	while (i > n && ft_strchr(set, s1[i -1]))
		i--;
	dest = malloc(i - n + 1);
	if (dest == NULL)
		return (NULL);
	ft_strlcpy(dest, &s1[n], i - n +1);
	return (dest);
}
