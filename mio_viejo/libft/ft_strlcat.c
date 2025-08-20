/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 11:01:10 by inbauman          #+#    #+#             */
/*   Updated: 2024/11/21 11:01:18 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include <stddef.h>

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	d;
	size_t	s;
	size_t	espacio_libre;

	d = ft_strlen(dst);
	s = ft_strlen(src);
	espacio_libre = size - d - 1;
	if (size <= d)
		return (size + s);
	if (espacio_libre > s)
		espacio_libre = s;
	ft_strlcpy(dst + d, src, espacio_libre +1);
	return (d + s);
}
