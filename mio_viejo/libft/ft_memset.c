/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:08:36 by inbauman          #+#    #+#             */
/*   Updated: 2024/11/21 10:08:38 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*esta función configura un bloque de memoria de n bytes con el valor 
especificado c, byte a byte, y devuelve el puntero al bloque de memoria 
inicial.*/
#include <stddef.h>

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*ptr;
	size_t			i;

	i = 0;
	ptr = (unsigned char *)s;
	while (i < n)
	{
		ptr[i] = c;
		i++;
	}
	return (s);
}
