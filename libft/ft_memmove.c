/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 11:55:13 by asalucci          #+#    #+#             */
/*   Updated: 2024/11/19 17:57:18 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*from;
	unsigned char	*to;
	int				i;

	from = (unsigned char *)src;
	to = (unsigned char *)dest;
	if (to > from)
	{
		i = (int)n - 1;
		while (i >= 0)
		{
			to[i] = from[i];
			i--;
		}
	}
	else
	{
		i = 0;
		while (i < (int)n)
		{
			to[i] = from[i];
			i++;
		}
	}
	return (dest);
}
