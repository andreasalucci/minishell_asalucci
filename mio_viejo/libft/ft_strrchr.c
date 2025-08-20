/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: inbauman <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:11:35 by inbauman          #+#    #+#             */
/*   Updated: 2024/11/21 16:11:37 by inbauman         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
 * ft_strrchr - Encuentra la última ocurrencia de un carácter en una cadena.
 *
 * Esta función busca la última aparición del carácter `c` en la cadena `s`.
 * El carácter `c` se interpreta como un valor de tipo unsigned char.
 *
 * Parámetros:
 *   - s: Puntero a la cadena donde se realizará la búsqueda.
 *   - c: Carácter a buscar, pasado como un entero.
 *
 * Retorno:
 *   - Un puntero a la última ocurrencia del carácter `c` en la cadena `s`.
 *   - Si el carácter `c` no se encuentra, retorna NULL.
 *   - Si `c` es '\0', retorna un puntero al terminador nulo de la cadena.
 *
 * Detalles:
 *   - La función recorre toda la cadena `s` y guarda un puntero a cada
 *     coincidencia del carácter `c`. Al finalizar, retorna la última
 *     coincidencia encontrada.
 */

#include <stdlib.h>

char	*ft_strrchr(const char *s, int c)
{
	const char		*coincidencias;
	unsigned char	uc;

	uc = (unsigned char)c;
	coincidencias = NULL;
	while (*s != '\0')
	{
		if (*s == (char)uc)
			coincidencias = s;
		s++;
	}
	if (uc == '\0')
		return ((char *)s);
	return ((char *)coincidencias);
}
