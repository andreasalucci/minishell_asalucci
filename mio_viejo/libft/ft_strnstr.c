/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 13:43:54 by inbauman          #+#    #+#             */
/*   Updated: 2024/11/27 13:43:56 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdio.h>
#include <string.h>

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	int		i;
	size_t	j;
	size_t	n;

	n = 0;
	if (*little == '\0')
	{
		return ((char *)big);
	}
	while (n < len && big[n] != '\0')
	{
		i = 0;
		j = n;
		while (j < len && big[j] == little[i] && big[j] != '\0')
		{
			j++;
			i++;
			if (little[i] == '\0')
				return ((char *)&big[n]);
		}
		n++;
	}
	return (NULL);
}
