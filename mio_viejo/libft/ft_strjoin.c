/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:38:59 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 17:39:08 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*dest;
	size_t	i;
	int		nbr;

	if (!s1 || !s2)
		return (NULL);
	nbr = 0;
	i = ft_strlen(s1) + ft_strlen(s2);
	dest = malloc(i +1);
	if (dest == NULL)
		return (NULL);
	while (*s1)
	{
		dest[nbr++] = *s1++;
	}
	while (*s2)
	{
		dest[nbr++] = *s2++;
	}
	dest[nbr] = '\0';
	return (dest);
}
