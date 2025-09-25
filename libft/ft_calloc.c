/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asnaider <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 09:43:57 by asalucci          #+#    #+#             */
/*   Updated: 2024/12/10 13:14:48 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	unsigned char	*str;
	size_t			prod;

	if (size == 0 || nmemb == 0)
		return (malloc(0));
	if (nmemb > ((size_t) -1) / size)
		return (NULL);
	prod = nmemb * size;
	str = (unsigned char *)malloc (prod);
	if (!str)
		return (NULL);
	while (prod > 0)
	{
		prod --;
		str[prod] = 0;
	}
	return (str);
}
