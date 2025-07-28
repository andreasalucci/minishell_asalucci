/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 10:04:04 by inbauman          #+#    #+#             */
/*   Updated: 2024/11/21 10:04:08 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*Función memcpy: La función memcpy copia n bytes de datos desde un bloque de 
memoria fuente (src) a un bloque de memoria destino (dest). Es una operación 
común en C para copiar bloques de memoria de manera rápida y eficiente.*/

#include <stddef.h>

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	char		*d;
	const char	*s;
	size_t		i;

	if (!dest && !src && n > 0)
		return (NULL);
	d = (char *)dest;
	s = (const char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dest);
}
