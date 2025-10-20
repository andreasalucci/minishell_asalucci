/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asalucci <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 15:31:41 by asalucci          #+#    #+#             */
/*   Updated: 2024/11/20 17:23:44 by asalucci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	dstlen;
	size_t	scrlen;

	dstlen = ft_strlen(dst);
	scrlen = ft_strlen(src);
	if (size)
	{
		if (ft_strlen(dst) >= size)
			return (size + scrlen);
		i = 0;
		while (src[i] != '\0' && i < size - dstlen - 1)
		{
			dst[dstlen + i] = src[i];
			i++;
		}
		dst[dstlen + i] = '\0';
		return (dstlen + scrlen);
	}
	else
		return (scrlen);
}
