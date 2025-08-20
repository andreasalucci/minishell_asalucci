/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 17:47:23 by inbauman          #+#    #+#             */
/*   Updated: 2024/12/05 17:47:58 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stddef.h>
#include <stdlib.h>

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t				i;
	const unsigned char	*s2;

	i = 0;
	s2 = (const unsigned char *)s;
	while (i < n)
	{
		if (s2[i] == (unsigned char)c)
			return ((void *)(s2 + i));
		i++;
	}
	return (NULL);
}
